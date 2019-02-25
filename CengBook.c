/* Explanations *************************************************************************************************************************************
|
| This file implements CengBook, which is a realistic simulation of a mini social-media application.
|
****************************************************************************************************************************************************/




/* Includes ****************************************************************************************************************************************/

#include "Utilities.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************************************************************************************/




/* Macro Definitions *******************************************************************************************************************************/

#define AUTHOR ( "Ceren Gursoy" )  /* Author's name */  /* TODO: Replace with your name */

/***************************************************************************************************************************************************/




/* Global Variable Definitions *********************************************************************************************************************/

BookPtr bookPtr ;

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

#if   defined(_WIN16) || defined(_WIN32) || defined(_WIN64)

  char operatingSystem[] = "Windows" ;  /* This is a Windows environment.                            */

#elif defined(__linux__)

  char operatingSystem[] = "Linux"   ;  /* This is a Linux environment (any GNU/Linux distribution). */

#elif defined(__APPLE__)

  char operatingSystem[] = "MacOSX"  ;  /* This is a Mac OS X environment.                           */

#endif

/***************************************************************************************************************************************************/




/* Functions ***************************************************************************************************************************************/

void printUsage ( String applicationName )
{
  NL ;

  printf( "Usage: %s <MaxNumberOfUsers> <MaxUserNameLength> <MaxNumberOfFriendsPerUser>\n\n" , applicationName ) ;
}

/*=================================================================================================================================================*/

void printBanner ( void )
{
  NL ;

  printf( "*******************************************************************************" "\n"          ) ;
  printf( "* CengBook                                                                    *" "\n"          ) ;
  printf( "* by %-72s *"                                                                    "\n" , AUTHOR ) ;
  printf( "*******************************************************************************" "\n"          ) ;
}

/*=================================================================================================================================================*/

void printMenu ( void )
{
  NL ;

  printf( "*******************************************************************************" "\n" ) ;
  printf( "Help                       : Print this menu"                                    "\n" ) ;
  printf( "List                       : List all users"                                     "\n" ) ;
  printf( "Plot                       : Create a plot of CengBook (requires GraphViz)"      "\n" ) ;
  printf( "Quit                       : Exit application"                                   "\n" ) ;
  printf( "-------------------------------------------------------------------------------" "\n" ) ;
  printf( "Register   <user>          : Create a new user and add it to CengBook"           "\n" ) ;
  printf( "Delete     <user>          : Delete user from CengBook"                          "\n" ) ;
  printf( "Find       <user>          : Search CengBook for user, display user information" "\n" ) ;
  printf( "Connect    <user1> <user2> : Add a connection from user1 to user2"               "\n" ) ;
  printf( "Disconnect <user1> <user2> : Remove connection from user1 to user2"              "\n" ) ;
  printf( "Path       <user1> <user2> : Find shortest path from user1 to user2"             "\n" ) ;
  printf( "-------------------------------------------------------------------------------" "\n" ) ;
  printf( "Sort <abc|pop> <asc|desc>  : Sort all users (alphabetically or by popularity)"   "\n" ) ;
  printf( "*******************************************************************************" "\n" ) ;
}

/*=================================================================================================================================================*/

void commandHelp ( void )
{
  printMenu();
}

/*=================================================================================================================================================*/

void commandList ( void )
{

  int i;

  UserPtr userPtr;

  if(bookPtr->numberOfUsers == 0){

     NL; printf("  CengBook is empty"); NL;}

  else {

       NL;

       for(i=0; i < bookPtr->numberOfUsers; i++){ 

           userPtr = getUserPtrByIndex( bookPtr , i ) ; 

           I2; printf("%s              : %u Follower(s) , %u Friend(s)",userPtr->name,userPtr->numberOfFollowers,userPtr->numberOfFriends);NL;}

       }

}
/*=================================================================================================================================================*/

void commandPlot ( void )
{
  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  UserPtr   userPtr             , friendPtr              ;
  int       userIndex           , friendIndex            ;
  String    runDotSystemCommand , openImageSystemCommand ;

  FILE    * file = fopen( "CengBook.dot" , "w" )         ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  fprintf( file , "digraph CengBook"                                                       "\n"                            ) ;
  fprintf( file , "{"                                                                      "\n"                            ) ;
  fprintf( file , "  graph [label=\"CengBook\\nNumber of users = %d\" , labelloc=t] ;"     "\n\n" , bookPtr->numberOfUsers ) ;
  fprintf( file , "  node  [shape=ellipse , fontcolor=black , color=red , penwidth=2.0] ;" "\n\n"                          ) ;
  fprintf( file , "  edge  [color=blue , penwidth=2.0] ;"                                  "\n\n"                          ) ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  for ( userIndex = 0 ; userIndex < bookPtr->numberOfUsers ; userIndex++ )
  {
    userPtr = bookPtr->userPointers[ userIndex ] ;

    fprintf( file , "  %s [label=\"%s\\n%d Friend(s)\\n%d Follower(s)\"] ;\n" ,
             userPtr->name                                                    ,
             userPtr->name                                                    ,
             userPtr->numberOfFriends                                         ,
             userPtr->numberOfFollowers                                       ) ;
  }

  if ( bookPtr->numberOfUsers > 0 )  { fprintf( file , "\n" ) ; }

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  for ( userIndex = 0 ; userIndex < bookPtr->numberOfUsers ; userIndex++ )
  {
    userPtr = bookPtr->userPointers[ userIndex ] ;

    for ( friendIndex = 0 ; friendIndex < userPtr->numberOfFriends ; friendIndex++ )
    {
      friendPtr = userPtr->friendPointers[ friendIndex ] ;

      fprintf( file , "  %s -> %s ;\n" , userPtr->name , friendPtr->name ) ;
    }
  }

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  fprintf( file , "}\n" ) ;
  fclose ( file         ) ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/

  if ( strcmp( operatingSystem , "Windows" ) == 0 )
  {
    runDotSystemCommand    = "CMD /C dot -Tgif CengBook.dot -o CengBook.gif" ;
    openImageSystemCommand = "CMD /C START CengBook.gif"                     ;
  }
  else  /* Assumption: Linux */
  {
    runDotSystemCommand    = "dot -Tgif CengBook.dot -o CengBook.gif"        ;
    openImageSystemCommand = "xdg-open CengBook.gif &"                       ;
  }

  if ( system( runDotSystemCommand    ) ) ;  /* "if" statement is used to eliminate warning about unused return value of "system" call */
  if ( system( openImageSystemCommand ) ) ;  /* "if" statement is used to eliminate warning about unused return value of "system" call */

  NL ;  I2 ;  printf( "Plot prepared\n" ) ;

  /*-----------------------------------------------------------------------------------------------------------------------------------------------*/
}

/*=================================================================================================================================================*/

void commandQuit ( void )
{
  
  deallocateBook(bookPtr);

  NL;  printf("  Have a nice day  \n");  NL;
 
  exit(0);

}

/*=================================================================================================================================================*/

void commandRegister ( String userName )
{
 
  UserPtr newuser;

  int   userindex;

  int maxNumberOfUsers=getMaxNumberOfUsers();

  int maxUserNameLength=getMaxUserNameLength();

  if( strlen(userName) > maxUserNameLength ) {NL;printf("  User name is longer than %d characters!",maxUserNameLength);NL;}

  else{

     if( getUserPtrByName(bookPtr,userName) != NULL ) 

        {NL; printf("  User '%s' already registered!", userName); NL; }

     else if( bookPtr->numberOfUsers == maxNumberOfUsers  ) 

        {NL; printf("  CengBook is full, cannot register any new users!"); NL;}

     else 

        { newuser = allocateUser(userName);

          bookPtr->numberOfUsers++ ;

          for(userindex=0; userindex < bookPtr->numberOfUsers; userindex++) {if( bookPtr->userPointers[userindex] == NULL)

									    { bookPtr->userPointers[userindex] =  newuser; break; }}

          NL; printf("  User '%s' registered successfully",userName); NL;

        }

      }

}

/*=================================================================================================================================================*/

void commandDelete ( String userName )
{
  UserPtr userPtr1, userPtr2, userDeleted;

  int i,k,userDeletedIndex,index;

  int isfound=0;

  if( bookPtr->userPointers == NULL){

  NL; printf("  User '%s' not found in CengBook!",userName); NL;}

  else
  {

      for(index=0; index < bookPtr->numberOfUsers; index++)
		
      {

      if(strcmp( getUserPtrByIndex(bookPtr,index)->name , userName ) == 0)

      {  userDeleted=getUserPtrByIndex(bookPtr,index); isfound=1; break; } 

      }

    if(isfound==0){ NL; printf("  User '%s' not found in CengBook!",userName); NL; }

    else 
      {

      for( i=0; i < bookPtr->numberOfUsers; i++) 
      {
      userPtr1 = bookPtr->userPointers[i];
        if( isConnected(userPtr1,userDeleted) == 1 )
           { if (removeConnection ( userPtr1 , userDeleted ) == 1 )
             {NL;printf("  User '%s' is removed from friends of user '%s'\n",userDeleted->name,userPtr1->name);NL;}
           }
      }

      for( k=0; k<userDeleted->numberOfFriends; k++)
      {
      userPtr2=getUserPtrByName(bookPtr, userDeleted->friendPointers[k]->name);
      userPtr2->numberOfFollowers--;
      printf("\n  User '%s' is discounted from followers of user '%s'\n",userDeleted->name,userPtr2->name);
      }

      for(userDeletedIndex = getUserIndexByPtr ( bookPtr , userDeleted ); userDeletedIndex<bookPtr->numberOfUsers-1; userDeletedIndex++)
      {
      bookPtr->userPointers[userDeletedIndex]=bookPtr->userPointers[userDeletedIndex+1];
      }
 
      deallocateUser(userDeleted);

      bookPtr->userPointers[ --(bookPtr->numberOfUsers) ] = NULL;

      NL; printf("  User '%s' is deleted from CengBook",userName); NL;

      }
  }
}

/*=================================================================================================================================================*/

void commandFind ( String userName )
{

  UserPtr userPtr;

  if( bookPtr->numberOfUsers == 0)

     { NL;  printf("  User '%s' not found in CengBook!",userName);  NL; }

  else{

      if(getUserPtrByName (bookPtr,userName )!=NULL)

      {  userPtr=getUserPtrByName(bookPtr,userName); 

         NL; printUser(userPtr); 
      }

      else { NL; printf("  User '%s' not found in CengBook!",userName); NL; }

      }

}

/*=================================================================================================================================================*/

void commandConnect ( String userName1 , String userName2 )
{

  int maxNumberOfFriendsPerUsers = getMaxNumberOfFriendsPerUser();

  UserPtr user1,user2;

  if( bookPtr->numberOfUsers == 0)

  { NL; printf("  User '%s' not found in CengBook!\n",userName1); printf("  User '%s' not found in CengBook!",userName2); NL; }

  else
  {

      if(getUserPtrByName(bookPtr,userName1) == NULL) {NL; printf("  User '%s' not found in CengBook!",userName1);NL;}

      else if(getUserPtrByName(bookPtr,userName2) == NULL) {NL; printf("  User '%s' not found in CengBook!",userName2);NL;}

      else

      {

      user1 = getUserPtrByName(bookPtr,userName1);
      user2 = getUserPtrByName(bookPtr,userName2);

      if (isConnected(user1,user2) == 1 ) { NL; printf("  User '%s' is already connected to user '%s'!",userName1,userName2); NL; }

      else if( strcmp( userName1 , userName2 ) == 0)  { NL; printf("  Connection to self is not allowed!"); NL; }

      else if( user1->numberOfFriends >= maxNumberOfFriendsPerUsers) { NL;printf("  Friend capacity of user '%s' is full!",user1->name);NL;}

      else
      {
        user1->friendPointers[user1->numberOfFriends] = user2;

	user1->numberOfFriends++ ;
        user2->numberOfFollowers++ ;
		
        NL; printf("  User '%s' successfully connected to user '%s' ", userName1,userName2); NL;
      }

      }

  }

}

/*=================================================================================================================================================*/

void commandDisconnect ( String userName1 , String userName2 )
{
	
  UserPtr user1,user2;

  if( bookPtr->numberOfUsers == 0)

  { NL; printf("  User '%s' not found in CengBook!\n",userName1); printf("  User '%s' not found in CengBook!",userName2); NL; }

  else
  {

      if(getUserPtrByName(bookPtr,userName1) == NULL) {NL; printf("  User '%s' not found in CengBook!",userName1);NL;}

      else if(getUserPtrByName(bookPtr,userName2) == NULL) {NL; printf("  User '%s' not found in CengBook!",userName2);NL;}

      else

      {

      user1 = getUserPtrByName(bookPtr,userName1);
      user2 = getUserPtrByName(bookPtr,userName2);

      if( strcmp( userName1 , userName2 ) == 0)  { NL; printf("  Disconnection from self is not allowed!"); NL; }

      else if(isConnected(user1,user2) == 0 ) { NL; printf("  User '%s' and user '%s' are not connected!",userName1,userName2); NL; }

      else
      {
        if(removeConnection(user1,user2) == 1)
		
        {NL; printf("  User '%s' successfully disconnected from user '%s'", userName1,userName2); NL;}
      }

      }

  }
 
}

/*=================================================================================================================================================*/

void commandPath ( String userName1 , String userName2 )
{

  UserPtrArray path ;
  int i;
  UserPtr user1,user2;

  if( bookPtr->userPointers == NULL)

  {NL; printf("  User '%s' not found in CengBook!\n",userName1); printf("  User '%s' not found in CengBook!",userName2); NL; }

  else
      {

      if(getUserPtrByName(bookPtr,userName1) == NULL) {NL; printf("  User '%s' not found in CengBook!",userName1);NL;}

      else if(getUserPtrByName(bookPtr,userName2) == NULL) {NL; printf("  User '%s' not found in CengBook!",userName2);NL;}

      else
      {

        user1 = getUserPtrByName(bookPtr,userName1);
        user2 = getUserPtrByName(bookPtr,userName2);

        if( strcmp( userName1 , userName2 ) == 0)  { NL; printf("  Path to self queried!"); NL; }

        else
        {
          path = shortestPath ( bookPtr , user1 , user2 ) ; 
	
          if( path != NULL) 

          {NL; for(i=0 ; path[i] != user2 ; i++) {printf("  '%s' ->",path[i]->name);} printf("  '%s'",path[i]->name); NL;}

          else { NL; printf("  There is no path from user '%s' to user '%s'",userName1,userName2); NL; }

        }
      }

      }       

}

/*=================================================================================================================================================*/

void commandSort ( String criterion , String order )
{
  ComparatorFunctionPointer compareFunctionPtr   ;
  boolean ascending                              ;   

  if( strcmp(criterion,"abc") == 0 || strcmp(criterion,"pop") == 0 )

  {
    if( strcmp(criterion,"abc") == 0 ) 

    { compareFunctionPtr = &compareByName; 

        if( strcmp(order,"asc") == 0 ) 

        { ascending = TRUE; sortBook ( bookPtr , compareFunctionPtr , ascending );
          NL; printf("  CengBook sorted 'alphabetically' in 'ascending' order"); NL;} 

        else if(strcmp(order,"desc") == 0 ) 

        { ascending = FALSE; sortBook ( bookPtr , compareFunctionPtr , ascending );
          NL; printf("  CengBook sorted 'alphabetically' in 'descending' order"); NL;} 

        else {NL;printf("  Invalid sort order '%s'!",order);NL;}
    }
  
    if( strcmp(criterion,"pop") == 0 ) 

    { compareFunctionPtr = &compareByPopularity; 

        if( strcmp(order,"asc") == 0 ) 

        { ascending = TRUE; sortBook ( bookPtr , compareFunctionPtr , ascending ); 
          NL; printf("  CengBook sorted 'by popularity' in 'ascending' order"); NL;} 

        else if(strcmp(order,"desc") == 0 ) 

        { ascending = FALSE; sortBook ( bookPtr , compareFunctionPtr , ascending ); 
          NL; printf("  CengBook sorted 'by popularity' in 'descending' order"); NL;} 


        else {NL;printf("  Invalid sort order '%s'!",order);NL;}
    }

  }

  else {NL; printf("  Invalid sort criterion '%s'!",criterion); NL;}

}

/*=================================================================================================================================================*/

void processInput ( String input )
{
  int         i                                                    ;
  int         numberOfTokens = 0                                   ;
  StringArray tokens         = tokenize( input , &numberOfTokens ) ;
  String      command                                              ;

  if ( numberOfTokens < 1 )  { return ; }

  command = tokens[0] ;

  for ( i = 0 ; i < strlen( command ) ; i++ )  { if ( ( command[i] >= 'A' ) && ( command[i] <= 'Z' ) )  { command[i] += 'a' - 'A' ; } }

  if      ( strcmp( command , "help"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandHelp      (                       ) ;  return ; } }
  else if ( strcmp( command , "list"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandList      (                       ) ;  return ; } }
  else if ( strcmp( command , "plot"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandPlot      (                       ) ;  return ; } }
  else if ( strcmp( command , "quit"       ) == 0 )  { if ( numberOfTokens == 1 ) { commandQuit      (                       ) ;  return ; } }
  else if ( strcmp( command , "register"   ) == 0 )  { if ( numberOfTokens == 2 ) { commandRegister  ( tokens[1]             ) ;  return ; } }
  else if ( strcmp( command , "delete"     ) == 0 )  { if ( numberOfTokens == 2 ) { commandDelete    ( tokens[1]             ) ;  return ; } }
  else if ( strcmp( command , "find"       ) == 0 )  { if ( numberOfTokens == 2 ) { commandFind      ( tokens[1]             ) ;  return ; } }
  else if ( strcmp( command , "connect"    ) == 0 )  { if ( numberOfTokens == 3 ) { commandConnect   ( tokens[1] , tokens[2] ) ;  return ; } }
  else if ( strcmp( command , "disconnect" ) == 0 )  { if ( numberOfTokens == 3 ) { commandDisconnect( tokens[1] , tokens[2] ) ;  return ; } }
  else if ( strcmp( command , "path"       ) == 0 )  { if ( numberOfTokens == 3 ) { commandPath      ( tokens[1] , tokens[2] ) ;  return ; } }
  else if ( strcmp( command , "sort"       ) == 0 )  { if ( numberOfTokens == 3 ) { commandSort      ( tokens[1] , tokens[2] ) ;  return ; } }
  else                                               { NL ;  I2 ;  printf( "Invalid command!\n" ) ;                               return ;   }

  /* If we reach here, the user must have provided incorrect number of arguments to a valid command */

  command[0] += 'A' - 'a' ;

  NL ;  I2 ;  printf( "Incorrect number of arguments provided for the command '%s'!\n" , command ) ;
}

/***************************************************************************************************************************************************/




/* Main Function ***********************************************************************************************************************************/

int main ( int argc , char * argv[] )
{
  char input [ MAX_INPUT_LENGTH ] ;

  if ( argc < 4 )  { printUsage( argv[0] ) ;  return 0 ; }

  configureCengBook( /* maxNumberOfUsers          */ atoi( argv[1] ) ,
                     /* maxUserNameLength         */ atoi( argv[2] ) ,
                     /* maxNumberOfFriendsPerUser */ atoi( argv[3] ) ) ;

  bookPtr = allocateBook() ;

  if ( bookPtr == NULL )  { NL ;  printf( "Error: Cannot allocate memory to store CengBook! Exiting!\n\n" ) ;  exit( 0 ) ; }

  printBanner() ;
  printMenu  () ;

  while ( TRUE )
  {
    NL                        ;  /* New line     */
    printf( "[CENGBOOK] > " ) ;  /* Print prompt */

    if ( fgets( input , MAX_INPUT_LENGTH , stdin ) == NULL )  /* Read input line, 'fgets' is safer than 'gets'. */
    {
      /* In the following two lines, the "if" statements surrounding the "freopen" function calls are used to eliminate warnings. */

      if   ( strcmp( operatingSystem , "Windows" ) == 0 )  { if ( freopen( "CON"      , "r" , stdin ) == NULL ) ; }

      else /* Assumption:               Linux          */  { if ( freopen( "/dev/tty" , "r" , stdin ) == NULL ) ; }

      continue ;
    }

    processInput( input ) ;  /* Tokenize and process the input command */
  }

  return 0 ;
}

/***************************************************************************************************************************************************/
