/*******************************************************************************/
/*  In order to use this configuration file for Indent (automatic reformating) */
/* puts this file in your project folder and use the cmd "indent filename.c".  */
/* Indent looks for a config file in your current folder and creats by default */
/* a backup file named filename.c~.                                            */
/*                                                                             */
/* Attention: Don't forget to add a space after each option	                   */
/*******************************************************************************/

/* -bad        a blank line is forced after every block of declarations */
/* -nbad (def) no change in source code */
-nbad 

/* -bap        a blank line is forced after every procedure body. */
/* -nbap (def) no change in source code */
-nbap 

/* -bbo        prefer to break long lines before boolean operators. */
/* -nbbo (def) no preferences */
-bbo 

/* -bc (def)   NEWLINE  is  forced after each comma in a declaration */
/* -nbc        no newline after comma in declaration */
-nbc

/* -bl         braces after statements are placed on the NEXT line */
/* -br (def)   braces after statements are placed on the SAME line */
-bl 

/* -blin       n specifies the number of spaces by which braces are indented */
/* -bli2 (def) */
-bli0 

/* -bls        put braces on the line after struct declaration lines */
/* -brs (def)  put braces on the same line as struct declarations */
-bls 

/* -ce (def)   put "else" on the same line than right brace */
/* -nce        put "else" on the next line after right brace */
-nce 

/* Changing the column for comments right of code (default for all 33) */
/* -cn         n specifies the number for code */
/* -cdn        n specifies the number for declarations */
/* -cpn        n specifies the number for preprocessor directives */
-c1 
-cd1 
-cp1 

/* -cdw        put "while" on the same line than right brace */
/* -ncdw       put "while" on the next line after right brace */
-cdw 

/* add a space after a cast operator */
-cs 

/* line up identifiers within declarations in the column specified */
/* 0 means directly after type (with one space) */
-di0 

/* -dj         left justifies declarations */
/* -ndj (def)  indents declarations the same as code */
-ndj 

/* -fc1        enables formatting of comments starting in first column */
/* -nfc1 (def) no changes for comments starting in first column */
-nfc1 

/* -fca        enables the formating of comments line (not box of comments) */
/* -nfca (def) no changes of comments */
-nfca 

/* -hnl        honor new lines */
/* -nhnl (def) do not honor new lines */
-hnl 

/* -in         n specifies the number of spaces for each code level */
/*             default value is 8 */
-i3 

/* max 80 characters per line */
--line-length100 

/* -lp (def)    line up continued lines at parentheses */
/* -nlp         do not line up at parentheses (ci has to be defined for this) /*
-lp 

/* -bfda        newline after each comma separating the arguments of a function declaration */
/* -nbfda (def) no new line(s) */
-nbfda 

/* -bfde        newline forced before the closing bracket of the function declaration */
/* -nbfde (def) no new line(s) */
-nbfde 

/* -cdb (def)   placement of comment delimiters on blank  lines
/* -ncdb        keep a line of comments instead of a box */
-ncdb 

/* Do not use TABs */
-nut 

/* adds 3 spaces between # and a block of preprocessor definition */
/* -ppi 3 */

/* -pcs        procedure calls and declarations will have extra space inserted */
/*             between the name and the '(' */
/* -npcs (def) no extra space */
-pcs 

/* -prs        parentheses are seperated with a space from the what is between them */
/* -nprs (def) no extra space */
-prs 

/* -psl (def)  types of procedures will be placed in seperate line before procedure */
/* -npsl       no seperate line for types of procedures */
-npsl 

/* -saf (def)  add a space between "for" and the following parenthesis */
/* -nsaf       add no space */
-saf 

/* -sai (def)  add a space between "if" and the following parenthesis */
/* -nsai       add no space */
-sai 

/* -saw (def)  add a space between "while" and the following parenthesis */
/* -nsaw       add no space */
-saw 

/* -sob        swallow optional blank lines */
/* -nsob (def) no changes in source code */
-nsob 

/* -sc         Enables the placement of asterisks (`*'s) at the left edge of all comments */
/* -nsc (def)  no changes in source code */
-nsc 

-lc80 
