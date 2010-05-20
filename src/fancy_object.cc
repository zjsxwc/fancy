#include "includes.h"

namespace fancy {

  FancyObject::FancyObject(Class_p _class) :
    _class(_class)
  {
    init_slots();
  }

  FancyObject::~FancyObject()
  {
  }

  Class_p FancyObject::get_class() const
  {
    return _class;
  }

  void FancyObject::set_class(Class_p klass)
  {
    if(klass) {
      _class = klass;
      init_slots();
    }
  }

  FancyObject_p FancyObject::get_slot(const string &slotname) const
  {
    object_map::const_iterator it = _slots.find(slotname);
    if(it != _slots.end()) {
      return it->second;
    } else {
      return nil;
    }
  }

  void FancyObject::set_slot(const string &slotname, const FancyObject_p value)
  {
    assert(value);
    _slots[slotname] = value;
  }

  void FancyObject::init_slots()
  {
    if(_class) {
      vector<string>::iterator it;
      for(it = _class->instance_slotnames().begin(); 
          it != _class->instance_slotnames().end();
          it++){
        _slots[*it] = nil;
      }
    }
  }

  FancyObject_p FancyObject::equal(const FancyObject_p other) const
  {
    if(!IS_CLASSINSTANCE(other))
      return nil;
  
    FancyObject_p other_instance = dynamic_cast<FancyObject_p>(other);

    if(_class->equal(other_instance->_class) != nil) {
      // TODO: compare slotvalues for both instances
      return nil;
    } else {
      return nil;
    }
  }

  FancyObject_p FancyObject::eval(Scope *scope)
  {
    return this;
  }

  OBJ_TYPE FancyObject::type() const
  {
    return OBJ_CLASSINSTANCE;
  }

  string FancyObject::to_s() const
  {
    return "<Unkown FancyObject>";
  }

  string FancyObject::inspect() const
  {
    return to_s();
  }

  FancyObject_p FancyObject::call_method(const string &method_name, FancyObject_p *arguments, int argc, Scope *scope)
  {
    Callable_p method = get_method(method_name);
    if(method) {
      if(argc == 0) {
        return method->call(this, scope);
      }
      return method->call(this, arguments, argc, scope);
    } else {
      // handle unkown messages, if unkown_message:with_params is defined
      if(Callable_p unkown_message_method = get_method("unknown_message:with_params:")) {
        int size = sizeof(arguments) / sizeof(arguments[0]);
        vector<FancyObject_p> arr_vec(arguments, &arguments[size]);
        FancyObject_p new_args[2] = { String::from_value(method_name), new Array(arr_vec) };
        return unkown_message_method->call(this, new_args, 2, scope);
      }
      
      // in this case no method is found and we raise a MethodNotFoundError
      FancyException_p except = new MethodNotFoundError(method_name, _class);
      throw except;
      return nil;
    }
  }

  FancyObject_p FancyObject::call_super_method(const string &method_name, FancyObject_p *arguments, int argc, Scope *scope)
  {
    if(Class_p superclass = _class->superclass()) {
      Callable_p method = superclass->find_method(method_name);
      if(method) {
        if(argc == 0) {
          return method->call(this, scope);
        }
        return method->call(this, arguments, argc, scope);
      } else {
        // handle unkown messages, if unkown_message:with_params is defined
        if(Callable_p unkown_message_method = _class->superclass()->find_method("unknown_message:with_params:")) {
          int size = sizeof(arguments) / sizeof(arguments[0]);
          vector<FancyObject_p> arr_vec(arguments, &arguments[size]);
          FancyObject_p new_args[2] = { String::from_value(method_name), new Array(arr_vec) };
          return unkown_message_method->call(this, new_args, 2, scope);
        }
      
        // in this case no method is found and we raise a MethodNotFoundError
        FancyException_p except = new MethodNotFoundError(method_name, _class->superclass());
        throw except;
        return nil;
      }
    } else {
      // TODO: create a UndefinedSuperClass exception class or so...
      error("No superclass defined for: ") << _class->to_s() << endl;
      return nil;
    }
  }

  void FancyObject::def_singleton_method(const string &name, Callable_p method)
  {
    assert(method);
    _singleton_methods[name] = method;
  }

  bool FancyObject::responds_to(const string &method_name)
  {
    if(get_method(method_name)) {
      return true;
    } else {
      return false;
    }
  }

  Callable_p FancyObject::get_method(const string &method_name)
  {
    // first of all, check singleton methods
    method_map::const_iterator it = _singleton_methods.find(method_name);
    if(it != _singleton_methods.end()) {
      return it->second;
    } else {
      return _class->find_method(method_name);
    }
  }

  string FancyObject::docstring() const
  {
    return _docstring;
  }

  void FancyObject::set_docstring(const string &docstring)
  {
    _docstring = docstring;
  }

  Array_p FancyObject::methods() const
  {
    vector<FancyObject_p> methods;
    
    for(map<string, Callable_p>::const_iterator it = _singleton_methods.begin();
        it != _singleton_methods.end();
        it++) {
      if(Method_p method = dynamic_cast<Method_p>(it->second)) {
        methods.push_back(method);
      } else if(NativeMethod_p method = dynamic_cast<NativeMethod_p>(it->second)) {
        methods.push_back(method);
      } else if(Block_p method = dynamic_cast<Block_p>(it->second)) {
        methods.push_back(method);
      }
    }
    vector<FancyObject_p> class_instance_methods = _class->instance_methods()->values();
    methods.insert(methods.end(), class_instance_methods.begin(), class_instance_methods.end());
    return new Array(methods);
  }
}
