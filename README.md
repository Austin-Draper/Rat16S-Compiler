# Rat16S-Compiler
THIS PROGRAM WAS UPLOADED TO GITHUB USING VISUAL STUDIO 2015 COMMUNITY EDITION

### I. 
This program is based on a language called "Rat16S" which is descibed in detail in section III. The Rat16S language is designed to be easy to understand. It has a short grammar and relatively clean semantics. 



### II. 
To run this program, you must have visual studio installed. In the repository, select the clone or download button and choose the "Download ZIP" option. Open the containing folder and open Project2.sln. From here you can now access the code and run the program. If you want to test the compiler yourself, open "text.txt" and alter the code however you would like.   

If you don't have visual studio or this method is giving you problems, there is an alternative. Simply navigate to the project2 folder and run Project2.exe. This will execute and run the program perfectly. From here you can still open "text.txt" and alter the code without the use of visual studio. After altering the code, run Project2.exe again and it will run your newly changed code. 



### III. 
### 1) Lexical Conventions: 
	
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


### 2) Syntax Rules

The following BNF describes the Rat16S as a set of rules.
For reference, Opt = optional and $$ = separator

R1. <Rat16S> ::= \<Opt Function Definitions>  
              $$ \<Opt Declaration List>  
              $$ \<Statement List>  
              $$  
R2. \<Opt Function Definitions> ::= \<Function Definitions> | \<Empty>  
R3. \<Function Definitions> ::= \<Function> | \<Function> \<Function Definitions>  
R4. \<Function> ::= function \<Identifier> \( \<Opt Parameter List> ) \<Opt Declaration List> \<Body>    
R5. \<Opt Parameter List> ::= \<Parameter List> | \<Empty>    
R6. \<Parameter List> ::= \<Parameter> | \<Parameter> , \<Parameter List>  
R7. \<Parameter> ::= \<IDs> : \<Qualifier>  
R8. \<Qualifier> ::= integer | boolean | real  
R9. \<Body> ::= \{ \<Statement List> }   
R10. \<Opt Declaration List> ::= \<Declaration List> | \<Empty>  
R11. \<Declaration List> ::= \<Declaration>; | \<Declaration> ; \<Declaration List>  
R12. \<Declaration> ::= \<Qualifier> \<IDs>  
R13. \<IDs> ::= \<Identifier> | \<Identifier>, \<IDs>  
R14. \<Statement List> ::= \<Statement> | \<Statement> \<Statement List>  
R15. \<Statement> ::= \<Compound> | \<Assign> | \<If> | \<Return> | \<Write> | \<Read> | \<While>  
R16. \<Compound> ::= \{ \<Statement List> }  
R17. \<Assign> ::= \<Identifier> := \<Expression>;  
R18. \<If> ::= if \( \<Condition> ) \<Statement> endif | if \( \<Condition> ) \<Statement> else \<Statement> endif  
R19. \<Return> ::= return ; | return \<Expression> ;  
R20. \<Write> ::= printf \( \<Expression> );  
R21. \<Read> ::= scanf \( \<IDs> );  
R22. \<While> ::= while \( \<Condition> ) \<Statement>  
R23. \<Condition> ::= \<Expression> \<Relop> \<Expresion>  
R24. \<Relop> ::= = | /= | > | < | => | <=  
R25. \<Expression> ::= \<Expression> + \<Term> | \<Expression> - \<Term> | \<Term>  
R26. \<Term> ::= \<Term> * \<Factor> | \<Term> / \<Factor> | \<Factor>  
R27. \<Factor> ::= - \<Primary> | \<Primary>  
R28. \<Primary> ::= \<Identifier> | \<Integer> | \<Identifier> \( \<IDs> ) | \( \<Expression> ) | \<Real> | true | false  
R29. \<Empty> ::= ɛ


### 3) Some Semantics

Rat16S is a conventional imperative programming language. A Rat16S program consists of a sequence of functions followed by the "main body" where the program executes.
All variables and functions must be declared before use.
Function arguments are passed by value.
There is an implied expressionless return at the end of all functions; the value returned by expressionless return statement is undefined.
Arithmetic expressions have their conventional meanings.
Integer division ignores any remainder.
Type casting is not allowed.
No arithmetic operations are allowed with booleans.


### 4) A Sample Rat16S Program

[* This is a comment. This sample code converts 
   Fahrenheit into Celcius *]

function convert (fahr:integer)  
{  
	return 5*(fahr-32)/9;  
}  

$$  
	integer low, high, step; [* declarations *]  
	
$$  
	scanf (low, high, step);  
	while (low < high)  
	{  
		printf (low);  
		Printf (convert (low));  
		low := low + step;  
	}  
$$  
