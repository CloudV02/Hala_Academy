#ifndef __TRY_LIB__
#define __TRY_LIB__

#include<stdio.h>
#include<setjmp.h>
#include<stdint.h>

/* global value */
static jmp_buf buf; // use for setjmp
static int exception_buf; // use for setjmp
static const char *error_message; // save value of string message error

typedef enum{
    NO_ERROR,
    FILE_ERROR,
    NETWORK_ERROR,
    CALCULATION_ERROR,
}ERROR_CODE;

static ERROR_CODE error; // save status of error_code



#define TRY if((exception_buf=setjmp(buf))==0)
#define CATCH(x) if(exception_buf==x)
#define THROW(ERROR_CODE,d_error) \
printf(d_error); \
error = ERROR_CODE; \
error_message = func_error_mess(error); \
longjmp(buf, (ERROR_CODE));


/* function return error of message when call Catch */
const char * func_error_mess(ERROR_CODE name_error);





#endif