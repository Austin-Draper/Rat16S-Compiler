# Rat16S-Compiler

1. explain compiler a little. 2. explain how to run it. 3. then explain in great detail
at end of 1, explain that an in depth description is in section 3 (III).
Section 2 just explains how to start the program. Use regular methods or just use the .exe (check of .exe works if change the text file... it should)

THIS PROGRAM WAS UPLOADED TO GITHUB USING VISUAL STUDIO 2015 COMMUNITY EDITION

I. This program is based on a language called "Rat16S" which is descibed in detail in section III. The Rat16S language is designed to be easy to understand. It has a short grammar and relatively clean semantics. 




II. To run this program, (test at home but it should just be to download as a .zip, unzip it, put it with other visual studio programs, then you can work with it from there.
Other option is just to do the same thing with downloading the .zip but instead of using visual studio just give directions to the .exe file.
assuming the .exe works, give directions to the text file being changed by the .exe file and explain to the user they can change it to their needs.

III. 
1) Lexical Conventions: 
	
The lexical units of a program are identifiers, keywords, integers, reals, operators, and other separators (total of 6 types of tokens). 
Blanks, tabs and newlines (collectively, "white space") as described below are ignored except as they serve to separate tokens. 
Some white space is required to separate otherise adjacent identifiers, keywords, reals and integers. 
<Identifier> is a sequence of letters and "_" (underscores).
The first character must be a letter and the last character CANNOT be a "_".
Upper and lower cases are read the same.
<Integer> is an unsigned decimal integer i.e., a sequence of decimal digits.
<Real> is an integer followed by "." and integer such as "123.0" or "." followed by an integer such as ".123".
Some identifiers are reserved for use as keywords, and may not be used otherwise. Some of these identifiers are: integer, if, else, endif, while, return, scanf, etc...
Comments are enclosed in [*          *]

2) Syntax Rules

The following BNF describes the Rat16S as a set of rules.
For reference, Opt = optional and $$ = separator

R1. <Rat16S> ::= <Opt Function Definitions>
              $$ <Opt Declaration List>
              $$ <Statement List>
              $$
R2. <Opt Function Definitions> ::= <Function Definitions> | <Empty>
R3. <Function Definitions> ::= <Function> | <Function> <Function Definitions>
R4. <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
R5. <Opt Parameter List> ::= <Parameter List> | <Empty>
R6. <Parameter List> ::= <Parameter> | <Parameter> , <Parameter List>
R7. <Parameter> ::= <IDs> : <Qualifier>
R8. <Qualifier> ::= integer | boolean | real
R9. <Body> ::= { <Statement List> } 
R10. <Opt Declaration List> ::= <Declaration List> | <Empty>
R11. <Declaration List> ::= <Declaration>; | <Declaration> ; <Declaration List>
R12. <Declaration> ::= <Qualifier> <IDs>
R13. <IDs> ::= <Identifier> | <Identifier>, <IDs>
R14. <Statement List> ::= <Statement> | <Statement> <Statement List>
R15. <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While>
R16. <Compound> ::= { <Statement List> }
R17. <Assign> ::= <Identifier> := <Expression>;
R18. <If> ::= if ( <Condition> ) <Statement> endif | if ( <Condition> ) <Statement> else <Statement> endif
R19. <Return> ::= return ; | return <Expression> ;
R20. <Write> ::= printf ( <Expression> );
R21. <Read> ::= scanf ( <IDs> );
R22. <While> ::= while ( <Condition> ) <Statement>
R23. <Condition> ::= <Expression> <Relop> <Expresion>
R24. <Relop> ::= = | /= | > | < | => | <=
R25. <Expression> ::= <Expression> + <Term> | <Expression> - <Term> | <Term>
R26. <Term> ::= <Term> * <Factor> | <Term> / <Factor> | <Factor>
R27. <Factor> ::= - <Primary> | <Primary>
R28. <Primary> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false
R29. <Empty> ::= COPY PASTE AN EPSILON HERE 


3) Some Semantics


4) A Sample Rat16S Program

[* This is a comment. This sample code converts 
   Fahrenheit into Celcius *]

function convert 
