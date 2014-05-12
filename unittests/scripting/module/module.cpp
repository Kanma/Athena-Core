#include <v8.h>

using namespace v8;


extern bool bind_Tests_Describable(Handle<Object> parent);


bool init_utils_submodule(Handle<Object> ns)
{
    return bind_Tests_Describable(ns);
}


bool init_test_module(Handle<Context> context)
{
    HandleScope handle_scope;

    Context::Scope context_scope(context);

    Handle<Object> ns = Object::New();
    context->Global()->Set(String::New("Tests"), ns);

    return init_utils_submodule(ns);
}
