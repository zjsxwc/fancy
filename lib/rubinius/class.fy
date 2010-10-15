def class Class {

  # When calling new: we have to call rbx's standard 'new' method
  # which then in turn calls the 'initialize' method.
  # This is the other part around the 'initialize' vs 'initialize:'
  # problem. See rbx/compiler/ast/method_def.rb for more information.
  def new: args {
    new: ~[args]
  }

  def Class superclass: superclass body: body_block {
    ruby: 'new args: [superclass] with_block: body_block
  }

  def define_method: name with: block {
    # define_method: ~[name, block]
    ruby: 'define_method args: [name] with_block: block
  }

  def undefine_method: name {
    remove_method: ~[name]
  }

  def define_class_method: name with: block {
    define_singleton_method: name with: block
  }

  def undefine_class_method: name {
    undefine_singleton_method: name
  }

  def alias_method: new for: old {
    alias_method: ~[new, old]
  }

  def subclass: body_block {
    Class superclass: self body: body
  }

  def nestes_classes {
    "Not Yet Implemented" raise!
  }
}