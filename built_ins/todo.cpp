// TODO
// i should pull new tokenizer code : //!DONE
// what i should do now :
// 1- implement builting in execution !!!!!!!!!	//! DONE
// 2- upgrade heredoc code 			!!!!!!!!! have some errors and linked to 7	//! DONE
// 3- extract envp before rederection	//! DONE
// 4- check for imbiguse rederictions	//! DONE
// 5- implement subshell				!!!!!!!!!!  //! DONE
// 6- handel exit status code we have five (also in built-in); //! DONE
// 7- remove paranteses in parsing  //! DONE
// 8- check for save derefrencing     //! DONE
// 9- test execuiton  //? in progress

// 10- test everything //TODO when expansion is finished


//! handel
// ```
// /tmp/hey$ unset a
// /tmp/hey$ $a ls
// minishell: command not found:
// ```

//! PROBLEMS

// we got a lot of problems that should be fixed
// if i have : export f="f" && echo $f
// if i exported before parsing it will change f to ""
// so i need to expand before runing the command

// first extract them enpv and let them in there linked list  then before runing export.... ,extract them to envp char ** ; then expand :::: this is a prblm soo
// TODO i will expand for each node of simple_cmd alone ......................... tomorow cus it will take a lot of time
// second export will add to the list , and we will do the same again
// export output e.g env content must not be completely empthy after unset ,it should have PWD and  SHLVL ...

//! FIX built-ins  :

// -nnnn for echo should be ignored even after first flag
// += append for export
// should pass NULL to cat in case of empthy expansion not '' or ""
// if runing export without args or with a comment it must show the env vars
// for cd it must if given more the one arg show err "to many args" and do nothing
// test more , and change terminal path
// pwd doesnt work at all it must print out the current working dir path ,and it must take a file descriptor in accoredens with the execution
// unset seems fine but need further testing and err handeling
// env is fine , should be tested with unset , and export
// last status need to be fixed in exit , testing with args
//! DO in echo handell case if write fails perror("minishell: ");
// in expansion if an expansion resulted a empthy string and nothing was linked to the arg expanded ,it must be removed rather then stored as a empthy str ,ask mahedi
// if i expand a file name in redirections and

//TODO EXIT IF MALLOC FAILED