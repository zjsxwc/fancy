#include "includes.h"

void init_block_class()
{  
  BlockClass->def_method("call", new NativeMethod("call", method_Block_call));
  BlockClass->def_method("call:", new NativeMethod("call:", method_Block_call_with_arg));
  BlockClass->def_method("while_true:", new NativeMethod("while_true:", method_Block_while_true));
  BlockClass->def_method("if:", new NativeMethod("if:", method_Block_if));
  BlockClass->def_method("unless:", new NativeMethod("unless:", method_Block_unless));
}


/**
 * Block instance methods
 */

FancyObject_p method_Block_call(FancyObject_p self, list<FancyObject_p> args, Scope *scope)
{
  if(args.size() != 0) {
    errorln("Block#call got more than 0 arguments!");
  } else {
    Block_p block = dynamic_cast<Block_p>(self);
    return block->call(self, args, scope);
  }
  return nil;
}

FancyObject_p method_Block_call_with_arg(FancyObject_p self, list<FancyObject_p> args, Scope *scope)
{
  if(args.size() != 1) {
    errorln("Block#call: didn't get an argument!");
  } else {
    Block_p block = dynamic_cast<Block_p>(self);
    NativeObject_p first_arg = args.front();
    list<FancyObject_p> passed_args;
    if(IS_ARRAY(first_arg)) {
      Array_p args_array = dynamic_cast<Array_p>(first_arg);
      int array_size = args_array->size();
      for(int i = 0; i < array_size; i++) {
        passed_args.push_back(args_array->at(i));
      }
    }
    return block->call(self, passed_args, scope);
  }
  return nil;
}

FancyObject_p method_Block_while_true(FancyObject_p self, list<FancyObject_p> args, Scope *scope)
{
  FancyObject_p first_arg = args.front();
  if(IS_BLOCK(first_arg)) {
    Block_p while_block = dynamic_cast<Block_p>(self);
    Block_p then_block = dynamic_cast<Block_p>(first_arg);
    list<FancyObject_p> empty;
    while(while_block->call(self, empty, scope) != nil) {
      then_block->call(self, empty, scope);
    }
    return nil;
  } else {
    return nil;
  }
}

FancyObject_p method_Block_if(FancyObject_p self, list<FancyObject_p> args, Scope *scope)
{
  FancyObject_p first_arg = args.front();
  if(first_arg != nil) {
    Block_p block = dynamic_cast<Block_p>(self);
    list<FancyObject_p> empty;
    return block->call(self, empty, scope);
  } else {
    return nil;
  }
}

FancyObject_p method_Block_unless(FancyObject_p self, list<FancyObject_p> args, Scope *scope)
{
  FancyObject_p first_arg = args.front();
  if(first_arg == nil) {
    Block_p block = dynamic_cast<Block_p>(self);
    list<FancyObject_p> empty;
    return block->call(self, empty, scope);
  } else {
    return nil;
  }
}
