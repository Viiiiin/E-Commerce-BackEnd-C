
#include "main.h"
#include <iostream>

int main()
{
Con2DB db1("localhost", "5432", "ecommerce", "47002", "db_ecommerce");

 PGresult *res;
 
  char buf[200];

  int pid;
  

#if (DEBUG > 0)
  setvbuf(stdout, (char*) NULL, _IONBF, 0);
  setvbuf(stderr, (char*) NULL, _IONBF, 0);
#endif


  traffic_light_type x, y;
  int t = 0;
  
  /* init random number generator  */
  srand((unsigned) time(NULL));


  pid = getpid();

 printf("Start main with pid %ld, ppid %ld \n",
	(long) pid, (long) getppid());

 printf(
"Format: time step, global time in second, elapsed time in sec, present time in nanoseconds, variable, value\n");

 init_logdb(db1);

 sprintf(sqlcmd,"SELECT table_name FROM information_schema.tables WHERE table_schema ='public'");
 res = db1.ExecSQLtuples(sqlcmd);
 int rows=PQntuples(res);
   
 for (int i=0; i<rows; ++i){
  std::cout << PQgetvalue(res,i,0) << std::endl;
 }
 PQclear(res);
}  /*  main()  */











/* **************************************************************** */
/*   NON USED   */
/* **************************************************************** */




#if 0


// fit curve of grades

#if 0
 
 // clean curvedgrade
   sprintf(sqlcmd, "DELETE FROM curvedgrade WHERE (courseID = %d) and (examday = \'%s\')",
	   courseID, myexamday);
     
   res = db1.ExecSQLcmd(sqlcmd);
    PQclear(res);
    

   // init
 min_rowgrade = 1000.0;


 while (min_rowgrade > 0)
   { // while computing curvedgrade
     
 // max rowgrade
 
     sprintf(sqlcmd, "SELECT MAX(rowgrade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s') and (rowgrade <= %lf)",
	     courseID, myexamday, min_rowgrade);
     
   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);

   if (rows <= 0)
     // no more rows
     {
       break;
     }

   
   max_rowgrade = atof(PQgetvalue(res, 0, 0));
   PQclear(res);

   // next to max rowgrade

   sprintf(sqlcmd, "SELECT * FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s') and (rowgrade < %lf)",
	   courseID, myexamday, max_rowgrade);

   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);
   PQclear(res);
   
   if (rows <= 0)
     // no more rows
     {
       min_rowgrade = max_rowgrade;
       break;
     }
   else  // rows > 0
     {
       sprintf(sqlcmd, "SELECT MAX(rowgrade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s') and (rowgrade < %lf)",
	   courseID, myexamday, max_rowgrade);
     
      res = db1.ExecSQLtuples(sqlcmd);
      rows = PQntuples(res);
      min_rowgrade = atof(PQgetvalue(res, 0, 0));
      PQclear(res);
     }
   
    
#if (DEBUG > 100)
   fprintf(stderr, "main():  min_rowgrade = %lf, max_rowgrade = %lf\n", min_rowgrade, max_rowgrade);
#endif  

	// interpolate between min_rowgrade and max_rowgrade
	
  // lowgrade
 
	sprintf(sqlcmd, "SELECT MAX(grade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s') and (rowgrade = %lf)",
		courseID, myexamday, min_rowgrade);
     

   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);

   lowgrade = atof(PQgetvalue(res, 0, 0));
 PQclear(res);

#if (DEBUG > 100)
	fprintf(stderr, "main():  lowgrade = %lf\n", lowgrade);
#endif
	

	// highgrade
 
	sprintf(sqlcmd, "SELECT DISTINCT grade FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s') and (rowgrade = %lf)",
		courseID, myexamday, max_rowgrade);
     
   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);

   highgrade = atof(PQgetvalue(res, 0, 0));
 PQclear(res);
 
#if (DEBUG > 100)
	fprintf(stderr, "main():  highgrade = %lf\n", highgrade);
#endif

	// fitting

   c0 = lowgrade;

   c1 =  (highgrade - lowgrade)/(max_rowgrade - min_rowgrade) ;


     
     
// print coefficients
 printf("Rowgrade Interval %lf : %lf; Grade Interval %lf : %lf: c0 = %lf, c1 = %lf\n",
	min_rowgrade, max_rowgrade, lowgrade, highgrade, c0, c1);

 // store values
      sprintf(sqlcmd, "BEGIN"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);
 
 for( RowGrade = min_rowgrade; RowGrade <= max_rowgrade; RowGrade++)
    {
      
   sprintf(sqlcmd,
"INSERT INTO CurvedGrade VALUES (%d, \'%s\', %lf, %d) ON CONFLICT DO NOTHING",
	   courseID, myexamday, RowGrade, MyRound(c0 + c1*(RowGrade - min_rowgrade)));

#if (DEBUG > 100)
	fprintf(stderr, "main(): insert grade (%d, \'%s\', %lf, %lf)  query: %s\n",
		courseID, myexamday, RowGrade, c0 + c1*(RowGrade - min_rowgrade),
		sqlcmd);
#endif
	
     res = db1.ExecSQLcmd(sqlcmd);
     PQclear(res);
   }

 
      sprintf(sqlcmd, "COMMIT"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);
      
   } // end while computing curvedgrade

 // padding up to rowgrade 100

 
 // max rowgrade
 
 sprintf(sqlcmd, "SELECT MAX(rowgrade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s')", courseID, myexamday);
     
   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);
   max_rowgrade = atof(PQgetvalue(res, 0, 0));
   PQclear(res);

  // max highgrade
 
   sprintf(sqlcmd, "SELECT MAX(grade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s') and (rowgrade = %lf)",
	   courseID, myexamday,  max_rowgrade);
     
   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);

   highgrade = atof(PQgetvalue(res, 0, 0));
   PQclear(res);


   // padding top values
   
// store values
      sprintf(sqlcmd, "BEGIN"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);
 
 for( RowGrade = max_rowgrade + 1; RowGrade <= 100; RowGrade++)
    {
      
   sprintf(sqlcmd,
"INSERT INTO CurvedGrade VALUES (%d, \'%s\', %lf, %lf) ON CONFLICT DO NOTHING",
	   courseID, myexamday, RowGrade, highgrade);

#if (DEBUG > 100)
	fprintf(stderr, "main(): padding insert grade (%d, \'%s\', %lf, %lf)  query: %s\n",
		courseID, myexamday, RowGrade, highgrade,
		sqlcmd);
#endif
	
     res = db1.ExecSQLcmd(sqlcmd);
     PQclear(res);
   }

 
      sprintf(sqlcmd, "COMMIT"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);
      

   // padding bottom values


 // min rowgrade
 
 sprintf(sqlcmd, "SELECT MIN(rowgrade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s')", courseID, myexamday);
     
   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);
   min_rowgrade = atof(PQgetvalue(res, 0, 0));
   PQclear(res);

  // min lowgrade
 
   sprintf(sqlcmd, "SELECT MIN(grade) FROM uncurvedgrade WHERE (CourseID = %d) and (ExamDay = '%s')",
	   courseID, myexamday);
     
   res = db1.ExecSQLtuples(sqlcmd);
   rows = PQntuples(res);
   lowgrade = atof(PQgetvalue(res, 0, 0));
   PQclear(res);

   
// store values
      sprintf(sqlcmd, "BEGIN"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);
 
 for( RowGrade = 0; RowGrade < min_rowgrade; RowGrade++)
    {
      
   sprintf(sqlcmd,
"INSERT INTO CurvedGrade VALUES (%d, \'%s\', %lf, %lf) ON CONFLICT DO NOTHING",
	   courseID, myexamday, RowGrade, lowgrade);

#if (DEBUG > 100)
	fprintf(stderr, "main(): padding insert grade (%d, \'%s\', %lf, %lf)  query: %s\n",
		courseID, myexamday, RowGrade, lowgrade,
		sqlcmd);
#endif
	
     res = db1.ExecSQLcmd(sqlcmd);
     PQclear(res);
   }

 
      sprintf(sqlcmd, "COMMIT"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);
#endif



#endif

