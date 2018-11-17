/*#include"a:\f3.c"    */
/*#include"a:\f2.c"  */
/*#include"a:\f4.c"*/
main()
{
   extern enterStr();
   extern void delStr();
   extern printStr();
   char c ;
   char str[80];
   enterStr(str);
   scanf("%c",&c);
   delStr(str,c);
   printStr(str);
}
