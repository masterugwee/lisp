#include "mpc.h"

int main(int argc, char** argv){
	mpc_parser_t* Adjective = mpc_or(4,mpc_sym("wow"),
			mpc_sym("so"),mpc_sym("many"),
			mpc_sym("such"));

	mpc_parser_t* Noun = mpc_or(5,mpc_sym("Dog"),
			mpc_sym("Cat"), mpc_sym("Varun"),
			mpc_sym("Carrot"), mpc_sym("Watch"));

	mpc_parser_t* Phrase = mpc_and(2,mpcf_strfold, Adjective,
			Noun ,free);

	mpc_parser_t* Doge = mpc_many(mpc_strfold, Phrase);


}
