#pragma once

int execute_builtin(char **argv);

int builtin_cd(char **argv);
int builtin_pwd(char **argv);
int builtin_exit(char **argv);

#ifdef HOMEWORK
int builtin_favs(char **argv);
int builtin_recordatorio(char **argv);
#endif
