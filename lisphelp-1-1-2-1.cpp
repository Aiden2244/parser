#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

/*****************************************************************
 *                     DECLARATIONS                              *
 *****************************************************************/
typedef int NUMBER;
typedef int NAME;
const int  NAMELENG = 20;      /* Maximum length of a name */
const int  MAXNAMES = 1000;     /* Maximum number of different names */
const int  MAXINPUT = 5000;     /* Maximum length of an input */
const char*   PROMPT = "-> ";
const char*   PROMPT2 = "> ";
const char  COMMENTCHAR = ';';
const int   TABCODE = 9;        /* in ASCII */

struct EXPLISTREC;
typedef EXPLISTREC* EXPLIST;
enum EXPTYPE {VALEXP,VAREXP,APEXP};
struct EXPREC
{
	  EXPTYPE etype; //what type of expression
	  NUMBER num;
	  NAME varble;
	  NAME optr;
	  EXPLIST args;
};
typedef EXPREC* EXP;

struct EXPLISTREC
{
	  EXP head;
	  EXPLIST tail;
};


struct VALUELISTREC
{
	  NUMBER  head;
	  VALUELISTREC*  tail;
};

typedef VALUELISTREC* VALUELIST;

struct NAMELISTREC
{
	  NAME   head;
	  NAMELISTREC* tail;
};
typedef NAMELISTREC* NAMELIST;

struct  ENVREC
{
	   NAMELIST vars;
	   VALUELIST values;
};

typedef ENVREC* ENV;

struct  FUNDEFREC
{
	   NAME  funname;
	   NAMELIST  formals;
	   EXP  body;
	   FUNDEFREC*  nextfundef;
};
typedef FUNDEFREC* FUNDEF;

FUNDEF  fundefs;

ENV globalEnv;

EXP currentExp;

char userinput[MAXINPUT];
int   inputleng, pos = 0;

const char*   printNames[MAXNAMES];
int   numNames, numBuiltins;

int   quittingtime;



/*****************************************************************
 *                     DATA STRUCTURE OP'S                       *
 *****************************************************************/

/* mkVALEXP - return an EXP of type VALEXP with num n            */

EXP mkVALEXP ( NUMBER n)
{
    EXP e = new EXPREC;
    e->etype = VALEXP;
    e->num = n;
   return e;
}/* mkVALEXP */


/* mkVAREXP - return an EXP of type VAREXP with varble nm  */

EXP mkVAREXP ( NAME nm)
{
    EXP e = new EXPREC;
    e->etype = VAREXP;
    e->varble = nm;
   
   return e;
}/* mkVAREXP */


/* mkAPEXP - return EXP of type APEXP w/ optr op and args el     */

EXP mkAPEXP (NAME op, EXPLIST el)
{
 
    EXP e = new EXPREC;
    e->etype = APEXP;
    e->optr = op;
    e->args = el;
   return e;
}/* mkAPEXP */

/* mkExplist - return an EXPLIST with head e and tail el         */

EXPLIST mkExplist (EXP e, EXPLIST el)
{
    EXPLIST newel  = new EXPLISTREC;
    newel->head = e;
    newel->tail = el;
    
    return newel;
}/* mkExplist */

/* mkNamelist - return a NAMELIST with head n and tail nl        */

NAMELIST mkNamelist ( NAME nm, NAMELIST nl)
{
   NAMELIST newnl = new NAMELISTREC;
    newnl->head = nm;
    newnl->tail = nl;
   return newnl;
}/* mkNamelist */

/* mkValuelist - return an VALUELIST with head n and tail vl     */

VALUELIST mkValuelist (NUMBER n,  VALUELIST vl)
{
   VALUELIST newvl;
   //do this
   return newvl;
}/* mkValuelist */

/* mkEnv - return an ENV with vars nl and values vl              */

ENV mkEnv ( NAMELIST nl, VALUELIST vl)
{
    ENV rho = new ENVREC;
    rho->vars = nl;
    rho->values = vl;
    return rho;
}/* mkEnv */

/* lengthVL - return length of VALUELIST vl      */

int lengthVL ( VALUELIST vl)
{
    int len = 0;
    while (vl != 0) {
        len++;
        vl = vl ->tail;
    }
    return len;
}/* lengthVL */

/* lengthNL - return length of NAMELIST nl    */

int lengthNL ( NAMELIST nl)
{
    int len = 0;
    while (nl != 0) {
        len++;
        nl = nl->tail;
    }
    return len;
}/* lengthNL */

/*****************************************************************
 *                     NAME MANAGEMENT                           *
 *****************************************************************/

/* fetchFun - get function definition of fname from fundefs */

FUNDEF fetchFun ( NAME fname)
{
    FUNDEF f = fundefs;
    while (f != 0 ){
        if (f->funname == fname)
            return f;
        f = f->nextfundef;
    }
    return 0;
        
}/* fetchFun */


/* newFunDef - add new function fname w/ parameters nl, body e   */

void  newFunDef (NAME fname,  NAMELIST nl, EXP e)
{
    FUNDEF f = new FUNDEFREC;
    f->funname = fname;
    f->formals = nl;
    f->body = e;
    f->nextfundef = fundefs;
    fundefs = f;
}// newFunDef


/* initNames - place all pre-defined names into printNames */

void initNames()
{
   int i = 1;
   fundefs = 0;
   printNames[i] = "if"; i++;
   printNames[i] = "while"; i++;
   printNames[i] = "set"; i++;
   printNames[i] = "begin"; i++;
   printNames[i] = "+"; i++;
   printNames[i] = "-"; i++;
   printNames[i] = "*"; i++;
   printNames[i] = "/"; i++;
   printNames[i] = "="; i++;
   printNames[i] = "<"; i++;
   printNames[i] = ">"; i++;
   printNames[i] = "print";
   numNames = i;
   numBuiltins = i;
}//initNames

/* install - insert new name into printNames  */

NAME install (char* nm)
{
   int i = 0;
   while (i <= numNames) {
      if (nm == printNames[i])
         break;
      i++;
   }
   if (i > numNames)
   {
      numNames = i;
      printNames[i] = nm;
   }
   return i;
}// install


/* prName - print name nm              */

void prName (NAME nm)
{
	 cout << printNames[nm];
} //prName

/*****************************************************************
 *                        INPUT                                  *
 *****************************************************************/

/* isDelim - check if c is a delimiter   */

int isDelim (char c)
{
   return ( ( c == '(') || ( c == ')') ||( c == ' ')||( c== COMMENTCHAR) );
}

/* skipblanks - return next non-blank position in userinput */

int skipblanks (int p)
{
   while (userinput[p] == ' ')
	++p;
   return p;
}


/* matches - check if string nm matches userinput[s .. s+leng]   */

int matches (int s, int leng,  const char* nm)
{
   int i=0;
   while (i < leng )
   {
	 if( userinput[s] != nm[i] )
	    return 0;
	 ++i;
	 ++s;
    }
   if (!isDelim(userinput[s]) )
	  return 0;
   return 1;
}/* matches */



/* nextchar - read next char - filter tabs and comments */

void nextchar (char& c)
{
    scanf("%c", &c);
    if (c == COMMENTCHAR )
    {
	  while ( c != '\n' )
		scanf("%c",&c);
    }
}


/* readParens - read char's, ignoring newlines, to matching ')' */
void readParens()
{
   int parencnt; /* current depth of parentheses */
   char c = '\0';
   parencnt = 1; // '(' just read
   do
   {
	  if (c == '\n')
	    cout <<PROMPT2;
	  nextchar(c);
	  pos++;
	  if (pos == MAXINPUT )
	  {
		cout <<"User input too long\n";
		exit(1);
	  }
	  if (c == '\n' )
		userinput[pos] = ' ';
	  else
		userinput[pos] = c;
	  if (c == '(')
		++parencnt;
	  if (c == ')')
	    parencnt--;
	}
    while (parencnt != 0 );
} //readParens

/* readInput - read char's into userinput */

void readInput()
{
    char  c;
    cout << PROMPT;
    pos = -1;
    do
	 {
	    ++pos ;
	    if (pos == MAXINPUT )
	    {
		    cout << "User input too long\n";
		    exit(1);
	    }
	    nextchar(c);
	    if (c == '\n' )
		   userinput[pos] = ' ';
	    else
		   userinput[pos] = c;
	    if (userinput[pos] == '(' )
		  readParens();
	 }
	while (c != '\n');
	inputleng = pos;
	userinput[pos+1] = COMMENTCHAR; // sentinel
}


/* reader - read char's into userinput; be sure input not blank  */

void reader ()
{
    do
    {
	  readInput();
	  pos = skipblanks(0);
    }
    while( pos > inputleng); // ignore blank lines
}

/* parseName - return (installed) NAME starting at userinput[pos]*/

NAME parseName()
{
   char nm[20]; // array to accumulate characters
   int leng; // length of name
   leng = 0;
   while ( (pos <= inputleng) && !isDelim(userinput[pos]) )
   {
	    
	    nm[leng] = userinput[pos];
	    ++pos;
	    ++leng;
   }
   if (leng == 0)
   {
	   cout<<"Error: expected name, instead read: "<< userinput[pos]<<endl;
	   exit(1);
   }
   nm[leng] = '\0';
   pos = skipblanks(pos); // skip blanks after name
  
   return ( install(nm) );
}// parseName

/* isDigits - check if sequence of digits begins at pos   */

int isDigits (int pos)
{
    while (userinput[pos] >= '0' && userinput[pos] <= '9')
        pos++;
    return isDelim(userinput[pos]);
}// isDigits


/* isNumber - check if a number begins at pos  */

int isNumber (int pos)
{
    if (userinput[pos] == '-')
        return isDigits(pos+1);
    return isDigits(pos);
}// isNumber

/* parseVal - return number starting at userinput[pos]   */

NUMBER parseVal()
{
    int sign = 1;
    int n = 0;
    if (userinput[pos] == '-') {
        sign = -1;
        pos++;
    }
    while (userinput[pos] == '0' && userinput[pos] <= '9') {
        n = 10*n + userinput[pos] - '0';
        pos++;
    }
    return sign*n;
}// parseVal

EXPLIST parseEL();

/* parseExp - return EXP starting at userinput[pos]  */

EXP parseExp()
{
    pos = skipblanks(pos);  // Skip initial whitespace
    if (userinput[pos] == '(' ) {
       pos = skipblanks(pos + 1);  // Skip whitespace after the opening parenthesis
       NAME op = parseName();
       pos = skipblanks(pos);  // Skip any whitespace after the parsed operator name
       EXPLIST el = parseEL();
       pos = skipblanks(pos);  // Skip any whitespace after the parsed expression list
       return mkAPEXP(op, el);
    }
    if (isNumber(pos)) {
       EXP e = mkVALEXP(parseVal());
       pos = skipblanks(pos);  // Skip any whitespace after the parsed value
       return e;
    }
    EXP e = mkVAREXP(parseName());
    pos = skipblanks(pos);  // Skip any whitespace after the parsed name
    return e;
}// parseExp


/* parseEL - return EXPLIST starting at userinput[pos]  */

EXPLIST parseEL()
{
    pos = skipblanks(pos);
    if (userinput[pos] == ')' ) {
      pos = skipblanks(pos + 1);
      cout << pos << endl;
      return 0;
    }
    EXP e = parseExp();
    if (e == nullptr) {  // Assuming parseExp() returns nullptr on failure
        // Handle error or break recursion
        return 0;
    }
    pos = skipblanks(pos);  // Ensure pos is updated correctly
    EXPLIST el = parseEL();
    return mkExplist(e, el); 
}// parseEL



/* parseNL - return NAMELIST starting at userinput[pos]  */

NAMELIST parseNL()
{
    pos = skipblanks(pos);
    if (userinput[pos] == ')') {
        pos = skipblanks(pos+1);
        return 0;
    }
    NAME nm = parseName();
    NAMELIST nl = parseNL();
    return mkNamelist(nm, nl);
}// parseNL

/* parseDef - parse function definition at userinput[pos]   */

NAME parseDef()
{
    NAME fname;        // function name
    NAMELIST nl;       // formal parameters
    EXP e;             // body
    pos = skipblanks(pos);
    pos = skipblanks(pos+1);
    pos = skipblanks(pos+6); //skip define
    fname = parseName();
    pos = skipblanks(pos);
    pos = skipblanks(pos+1); //skip '('
    nl = parseNL();
    e = parseExp();
    newFunDef(fname, nl, e);
   return ( fname);
}// parseDef

/*****************************************************************
 *                     ENVIRONMENTS                              *
 *****************************************************************/

/* emptyEnv - return an environment with no bindings */

ENV emptyEnv()
{
   return  mkEnv(0, 0);
}

/* bindVar - bind variable nm to value n in environment rho */

void bindVar ( NAME nm,  NUMBER n,  ENV rho)
{
   rho->vars = mkNamelist(nm, rho->vars);
   rho->values = mkValuelist(n, rho->values);
}

/* findVar - look up nm in rho   */

VALUELIST findVar ( NAME nm, ENV rho)
{
  
    NAMELIST varList = rho->vars;
    VALUELIST valList = rho->values;

    // Iterate through the environment
    while (varList != nullptr && valList != nullptr)
    {
        if (varList->head == nm)  // If the variable is found
        {
            return valList;  // Return its associated value
        }
        varList = varList->tail;
        valList = valList->tail;
    }

    // Variable not found
    return nullptr;
  
}


/* assign - assign value n to variable nm in rho   */

void  assign (NAME nm,  NUMBER n, ENV rho)
{
   VALUELIST varloc;
   varloc = findVar(nm, rho);
   varloc->head = n;
}// assign

/* fetch - return number bound to nm in rho */

NUMBER fetch ( NAME nm, ENV rho)
{
   VALUELIST  vl;
   vl = findVar(nm, rho);
   return (vl->head);
}

/* isBound - check if nm is bound in rho  */

int isBound ( NAME nm, ENV rho)
{
   return ( findVar(nm, rho) != 0 );
}


/*****************************************************************
 *                     NUMBERS                                   *
 *****************************************************************/

/* prValue - print number n    */

void prValue ( NUMBER n )
{
   cout << n;
}
// prValue


/* isTrueVal - return true if n is a true (non-zero) value  */

int isTrueVal ( NUMBER n)
{
   return ( n != 0 );
}// isTrueVal


/* arity - return number of arguments expected by op  */

int arity ( int op)
{
	 if ( ( op > 4) && (op < 12) )
	    return 2;
	 return  1;
}// arity

/* applyValueOp - apply VALUEOP op to arguments in VALUELIST vl */

NUMBER applyValueOp(int op, VALUELIST vl)
{
    // Ensure there are enough arguments in the VALUELIST for the operation.
    int argCount = lengthVL(vl);
    if (argCount < arity(op))
    {
        cout << "Error: insufficient arguments for operation" << endl;
        exit(1);
    }

    // Access the arguments from the VALUELIST.
    NUMBER arg1 = vl->head;
    NUMBER arg2 = 0;
    if (argCount > 1)
        arg2 = vl->tail->head;

    // Perform the specified operation.
    switch (op)
    {
    case 5: 
        return arg1 + arg2;
    case 6: 
        return arg1 - arg2;
    case 7: 
        return arg1 * arg2;
    case 8: 
        if (arg2 == 0)
        {
            cout << "Error: division by zero" << endl;
            exit(1);
        }
        return arg1 / arg2;
    // ... other cases for other operators
    case 9:
        return arg1 == arg2;
    case 10:
        return arg1 > arg2;
    case 11:
        return arg1 < arg2;
    default:
        cout << "Error: unknown operator" << endl;
        exit(1);
    }
}
 // applyValueOp


/*****************************************************************
 *                     EVALUATION                                *
 *****************************************************************/

// eval - return value of expression e in local environment rho
NUMBER eval(EXP e, ENV rho);

/* evalList - evaluate each expression in el */
VALUELIST evalList(EXPLIST el, ENV rho)
{
    // Base case: if the expression list is empty, return an empty value list
    if (el == nullptr)
    {
        return nullptr;
    }

    // Recursive case: evaluate the head expression, then recursively evaluate the tail
    NUMBER headValue = eval(el->head, rho);  // Evaluate the head expression
    VALUELIST tailValues = evalList(el->tail, rho);  // Recursively evaluate the tail

    // Construct a new value list node with the evaluated head value and the evaluated tail values
    VALUELIST newValueList = mkValuelist(headValue, tailValues);

    return newValueList;
}


/* applyUserFun - look up definition of nm and apply to actuals  */

NUMBER applyUserFun(NAME fname, VALUELIST actuals)
{
    // Look up the function definition in the global function definitions
    FUNDEF f = fetchFun(fname);

    // Check if the function definition was found
    if (f == nullptr)
    {
        cout << "Error: undefined function " << printNames[fname] << endl;
        exit(1);
    }

    // Check if the number of actual arguments matches the number of formal parameters
    NAMELIST formals = f->formals;
    if (lengthVL(actuals) != lengthNL(formals))
    {
        cout << "Error: argument count mismatch for function " << printNames[fname] << endl;
        exit(1);
    }

    // Create a new environment with bindings for the formal parameters to the actual arguments
    ENV newEnv = mkEnv(nullptr, nullptr);
    while (formals != nullptr && actuals != nullptr)
    {
        bindVar(formals->head, actuals->head, newEnv);
        formals = formals->tail;
        actuals = actuals->tail;
    }

    // Evaluate the function body in the new environment
    NUMBER result = eval(f->body, newEnv);

    return result;
}

 // applyUserFun

/* applyCtrlOp - apply CONTROLOP op to args in rho   */
NUMBER applyCtrlOp(int op, EXPLIST args, ENV rho)
{
    if (args->tail == nullptr) {
        cout << "Error: args should have something following it" << endl;
        exit(1);
    }

    switch(op)
    {
        case 1:  // "if" operator
        {
            // Evaluate the condition
            NUMBER condVal = eval(args->head, rho);
            args = args->tail;

            // If condition is true, evaluate the first branch, else evaluate the second branch
            if (isTrueVal(condVal))
                return eval(args->head, rho);
            else
                return eval(args->tail->head, rho);
        }
        case 2:  // "while" operator
        {
            // Extract the condition and body expressions
            EXP condExp = args->head;
            EXP bodyExp = args->tail->head;

            // While the condition is true, evaluate the body
            while (isTrueVal(eval(condExp, rho)))
                eval(bodyExp, rho);

            return 0;  // Return 0 after the loop exits
        }
        case 3:  // "set" operator
        {
            // Extract the variable name and the new value expression
            NAME varName = args->head->varble;
            EXP newValExp = args->tail->head;

            // Evaluate the new value expression and assign it to the variable
            NUMBER newVal = eval(newValExp, rho);
            assign(varName, newVal, rho);

            return newVal;  // Return the new value
        }
        case 4:  // "begin" operator
        {
            // Evaluate each expression in the list, returning the value of the last one
            NUMBER lastVal = 0;
            while (args != nullptr)
            {
                lastVal = eval(args->head, rho);
                args = args->tail;
            }

            return lastVal;
        }
        case 12:  // "print" operator
        {
            // Evaluate the expression to be printed and print it
            NUMBER val = eval(args->head, rho);
            prValue(val);
            cout << endl;

            return val;  // Return the value that was printed
        }
        default:
            cout << "Error: unknown control operator" << endl;
            exit(1);
    }
}// applyCtrlOp

NUMBER eval(EXP e, ENV rho)
{
    switch (e->etype)
    {
        case VALEXP:
            return e->num;  // Return the number directly for value expressions.
        case VAREXP:
        {
            // Look up the variable in the environment and return its value.
            VALUELIST vl = findVar(e->varble, rho);
            if (vl != nullptr)
                return vl->head;
            else
            {
                cout << "Error: unbound variable" << endl;
                exit(1);
            }
            break;
        }
        case APEXP:
        {
            // Evaluate the operator and arguments, then apply the operator to the arguments.
            NAME op = e->optr;
            EXPLIST args = e->args;
            VALUELIST argValues = evalList(args, rho);
            return applyValueOp(op - '0', argValues);
        }
        default:
            cout << "Error: unknown expression type" << endl;
            exit(1);
    }
}


/*****************************************************************
 *                     READ-EVAL-PRINT LOOP                      *
 *****************************************************************/

int main()
{
   initNames();
   globalEnv = emptyEnv();

   quittingtime = 0;
   while (!quittingtime)
   {
	 reader();
	 if ( matches(pos, 4, "quit"))
	    quittingtime = 1;
	 else if( (userinput[pos] == '(') &&
		    matches(skipblanks(pos+1), 6, "define")  )
	 {
		    prName(parseDef());
		    cout <<endl;
	 }
	 else {
			currentExp = parseExp();
			prValue(eval(currentExp, emptyEnv() ));
			cout <<endl<<endl;
		 }
	}// while
  return 0;
}