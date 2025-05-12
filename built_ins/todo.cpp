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



//! PROBLEMS

// second export will add to the list , and we will do the same again
// export output e.g env content must not be completely empthy after unset ,it should have PWD and  SHLVL ...

//! FIX built-ins  :

// += append for export
// should pass NULL to cat in case of empthy expansion not '' or ""
// if runing export without args or with a comment it must show the env vars
// unset seems fine but need further testing and err handeling

//TODO EXIT IF MALLOC FAILED