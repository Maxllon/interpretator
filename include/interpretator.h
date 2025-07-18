#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include<expression.h>
#include<dorl_object.h>

dorl_object*
interpretate(expression* AST);

#endif