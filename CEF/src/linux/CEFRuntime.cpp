#include "include/capi/cef_app_capi.h"
#include "include/base/cef_compiler_specific.h"

NO_STACK_PROTECTOR
int main(int argc, char* argv[])
{
    cef_main_args_t cef_arg{ argc, argv };
    return cef_execute_process(&cef_arg, nullptr, nullptr);
}
