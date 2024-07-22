#include "try_catch.h"

/* function of return name CATCH */
const char * func_error_mess(ERROR_CODE name_error){
    
    switch(name_error)
    {
        case FILE_ERROR:
            return "FILE_ERROR";
            break;
        case NETWORK_ERROR:
            return "NETWORK_ERROR";
            break;
        case CALCULATION_ERROR:
            return "CALCULATION_ERROR";
            break;
    } 

}