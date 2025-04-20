#include "std.h"
#include "nodes.h"



//////////////////
// The program! //
//////////////////
Environ *ProgNode::semant( Environ *e ){
file_lab=genLabel();

a_ptr<Environ> env( d_new Environ(genLabel(),Type::int_type,0,e ) );

stmts->semant(env);
Environ *sem_env;
sem_env=env.release();
return sem_env;


}

