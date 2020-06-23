#ifndef UTILS_H
#define UTILS_H

void Show_Error(const char* messageFormat, ...);

const char* Get_Env_Var_Or_Default(const char* envVarName, const char* defaultValue);

int Parse_Number(char character);

#endif
