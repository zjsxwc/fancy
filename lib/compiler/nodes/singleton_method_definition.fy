def class AST {
  def class SingletonMethodDefinition : Node {
    read_slots: ['object_ident, 'method]

    def is_protected: p {
      @method is_protected: p
    }

    def is_private: p {
      @method is_private: p
    }

    def init_docstring {
      @docstring = @method docstring
    }

    def initialize: obj_ident method: method {
      @object_ident = obj_ident
      @method = method
      @init_docstring
    }

    def SingletonMethodDefinition from_sexp: sexp {
      obj_ident = sexp second to_ast
      method = sexp third to_ast
      SingletonMethodDefinition new: obj_ident method: method
    }
  }
}