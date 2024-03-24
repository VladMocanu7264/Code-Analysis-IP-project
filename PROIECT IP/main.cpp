#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <graphics.h>
using namespace std;

struct nodA;
typedef nodA* arbore;
///liste de arbori
struct nodL
{
	arbore fiu;
	nodL *urm;
};
struct lista
{
	nodL *prim;
	nodL *ultim;
	int lg;
};

///arbori
struct nodA
{
	int tip;
	string instr;
	lista L;
};
/*
1-instr
2-cond
3-if
4-else
5-while
6-for
7-do
8-break
9-continue
10-switch
11-case
*/

void initLista(lista &L)
{
	L.prim=L.ultim=NULL;
	L.lg=0;
}

arbore IntrodInArb(arbore &A, int tip, string instr)
{
	///cream fiul
	arbore F = new nodA;
	F->instr=instr;
	F->tip=tip;
	initLista(F->L);
	///cream intrarea in lista
	nodL *nou = new nodL;
	nou->fiu=F;
	nou->urm=NULL;

	///daca ar fi primul fiu
	if(A->L.lg == 0)
	{
    	A->L.prim=nou;
    	A->L.ultim=nou;
    	A->L.lg++;
	}
	else///adaugam la capat
	{
    	A->L.ultim->urm=nou;
    	A->L.ultim=nou;
    	A->L.lg++;
	}

	return F;
}

///A
ifstream fin1("text.in");
ifstream fin2("text2.in");
ifstream fin3("text3.in");
ifstream fin4("text4.in");

void citire(string &cod, int nr)
{
	cod="";
	string s;
	switch(nr)
	{
	case 1:
    	while(fin1>>s)
        	cod+=s;
    	break;
	case 2:
    	while(fin2>>s)
        	cod+=s;
    	break;
	case 3:
    	while(fin3>>s)
        	cod+=s;
    	break;
	case 4:
    	while(fin4>>s)
        	cod+=s;
    	break;
	}
	cod+="   ";
}
///A

int nrFunctiiPredef=12;
string functiiPredef[12]=
{
	"", "", "", "if", "else", "while", "for", "do", "break;", "continue;", "switch", "case"
};

int nrCuvinteDictionar=10;
string dictionar[25]= {"void", "int", "float", "double", "char", "string", "return", "bool", "unsigned", "struct"};

void TransfInArb(arbore &A, string &cod, int st, int dr)
{
	if(st<=dr)
	{
    	///Debugging
    	//cout<<cod.substr(st, dr-st+1)<<' '<<st<<' '<<dr<<'\n';

    	///1-functie predefinita
    	///2-instructiune
    	///3-conditie

    	int copieSt=st;
    	///identificam instructiunea principala a subarborelui
    	string instr="";
    	while(cod[st]!='(' && cod[st]!='{' && st<=dr)
    	{
        	if((cod[st]=='-' && cod[st-1]=='-')||(cod[st]==39 && cod[st-1]==39))
            	instr+=' ';
        	if(cod[st]!=' ')
            	instr+=cod[st];
        	++st;
    	}

    	///adaugam nodul in arborele nostru
    	///este functie predef?
    	int i=1;
    	for(i=3; i<nrFunctiiPredef && functiiPredef[i]!=instr; ++i);

    	arbore F;
    	///1
    	if(i<nrFunctiiPredef)
        	F=IntrodInArb(A, i, instr); /// CAZ DE BAZA
    	else if(instr.substr(0, 4)=="else") /// verifica daca primele caractere sunt else
    	{
        	F=IntrodInArb(A, 4, "else"); /// CAZ DE BAZA
        	st=copieSt+4;
    	}
    	else if(instr.substr(0, 4)=="case") /// verifica daca primele caractere sunt case
    	{
        	F=IntrodInArb(A, 11, "case"); /// CAZ DE BAZA
        	st=copieSt+4;
    	}
    	else if(instr.substr(0, 7)=="default") /// verifica posibilul default
    	{
        	F=IntrodInArb(A, 11, "default"); /// CAZ DE BAZA
        	st=copieSt+7;
    	}
    	///2
    	else if(cod[dr]==';')///instructiune clasica
    	{
        	///adaugam eventualul rest al atribuirii a=b+(c-2)/2;, in instructiune va fi a=b+ deci trebuie adaugat si restul
        	for(; st<=dr; ++st)
            	instr+=cod[st];
        	for(int i=0; i<nrCuvinteDictionar; i++)
            	if(dictionar[i]==instr.substr(0, dictionar[i].length()))
            	{
                	if(i==9)///struct
                	{
                    	int x=1;
                    	while(instr[5+x]!='{' && instr[5+x]!=';') ++x;
                    	dictionar[++nrCuvinteDictionar-1]=instr.substr(6, x-1);

                	}
                	instr.insert(dictionar[i].length(), " ");
            	}
        	F=IntrodInArb(A, 1, instr); /// CAZ DE BAZA
    	}
    	///3
    	else///este conditie a unei instructiuni
    	{
        	///adaugam eventualul rest al conditiei
        	for(; st<=dr; ++st)
            	instr+=cod[st];
        	for(int i=0; i<nrCuvinteDictionar; i++)
            	if(dictionar[i]==instr.substr(0, dictionar[i].length()))
                	instr.insert(dictionar[i].length(), " ");
        	F=IntrodInArb(A, 2, instr); /// CAZ DE BAZA
    	}

    	///separam restul codului si il prelucram
    	if(st<dr)///mai avem de prelucrat, suntem in interiorul unei functii predefinite
    	{
        	///conditia for/if/while
        	if(cod[st]=='(')
        	{
            	///inaintam pana la echilibrearea nr( si nr)
            	int sf=st++;
            	int nrp=1;
            	bool OKa=0;
            	bool OKg=0;
            	while(nrp>0)
            	{
                	++sf;
                	if(cod[sf]==39 && !OKg)
                    	OKa=!OKa;
                	else if(cod[sf]=='"' && !OKa)
                    	OKg=!OKg;
                	else if(OKa ||OKg){}
                	else if(cod[sf]=='(')
                    	++nrp;
                	else if(cod[sf]==')')
                    	--nrp;
            	}

            	TransfInArb(F, cod, st, sf-1); /// apelarea pentru conditie

            	///trecem la instructiunile ce se intampla in for/if/while
            	st=sf+1;
        	}
        	else if(F->tip==11)
        	{
            	int sf=st;
            	while(cod[sf]!=':') ++sf;

            	TransfInArb(F, cod, st, sf-1);
            	st=sf;
        	}

        	/// prelucram ce este intre acolade
        	if(cod[st]=='{' || F->tip==11)
        	{
            	if(F->tip==10)///este switch
            	{
                	++st;

                	bool sfSwitch=0;
                	///impartim pe caseuri
                	for(int i=st; i<=dr && !sfSwitch; ++i)
                	{
                    	///C1: pana la ;
                    	///C2: pana cand nr{ = nr}
                    	///C3: pana cand nr( = nr)

                    	int nrp=0;///(
                    	int nrP=0;///{
                    	bool OKg=0;///"
                    	bool OKa=0;///'
                    	if(!OKa || cod[i]==39)
                        	if(!OKg || cod[i]=='"')
                        	{
                            	switch(cod[i])
                            	{
                            	case '"':
                                	OKg=!OKg;
                                	break;
                            	case 39:
                                	OKa=!OKa;
                                	break;
                            	case '(':
                                	++nrp;
                                	break;
                            	case ')':
                                	--nrp;
                                	break;
                            	case '{':
                                	++nrP;
                                	break;
                            	case '}':
                                	--nrP;
                                	///sf de acolada de for/while/if
                                	if(nrP==0 && !OKg &&
                                	(cod.substr(i+1, 4)=="case" || cod.substr(i+1, 7)=="default"))
                                	{
                                    	TransfInArb(F, cod, st, i);
                                    	st=i+1;
                                	}
                                	break;
                            	case ';':
                                	if(nrp==0 && nrP==0 && !OKg &&
                                	(cod.substr(i+1, 4)=="case" || cod.substr(i+1, 7)=="default")) /// probabil o instuctiune simpla
                                	{
                                    	TransfInArb(F, cod, st, i);
                                    	st=i+1;
                                	}
                                	break;
                            	}
                        	}

                    	if(i==dr)///sf switchului
                        	TransfInArb(F, cod, st, dr);
                	}
            	}
            	else///este while/for/do_while/case
            	{
                	///C1: pana la ;
                	///C2: pana cand nr{ = nr}
                	///C3: pana cand nr( = nr)

                	int nrp=0;///(
                	int nrP=0;///{
                	bool OKg=0;///"
                	bool OKa=0;///'
                	int nrIfElse=0;///contor ce creste la if si scade la else
                	bool OKif=0;

                	++st;
                	///impartirea pe bucati
                	for(int i=st; i<=dr; ++i)
                	{
                    	if(!OKa || cod[i]==39)
                        	if(!OKg || cod[i]=='"')
                            	switch(cod[i])
                            	{
                            	case 'i':
                                	if(cod.substr(i, 3)=="if(" && nrP==0)
                                    	++nrIfElse;
                                	break;
                            	case '"':
                                	OKg=!OKg;
                                	break;
                            	case 39:
                                	OKa=!OKa;
                                	break;
                            	case '(':
                                	++nrp;
                                	break;
                            	case ')':
                                	--nrp;
                                	break;
                            	case '{':
                                	++nrP;
                                	break;
                            	case '}':
                                	--nrP;
                                	if(nrP==0 && cod[i+1]!=';')///sf de acolada de for/while/if
                                	{
                                    	///am terminat acolada, dar trebuie sa verificam daca nu e cumva un do while in alta instructiune
                                    	if(cod.substr(i+1, 6)=="while(")///primul pas
                                    	{
                                        	///acum cautam sa vedem daca dupa acel while
                                        	int x=i+7;
                                        	while(cod[x]!=')') ++x;
                                        	///daca dupa ) avem ; inseamna ca avem un do while
                                        	///dar daca nu incepem cu el chunkul atunci este singura instructiune
                                        	///a unei instructiuni fara acolade
                                        	///deci trebuie sa luam cu tot cu while
                                        	if(cod[x+1]==';' && cod.substr(st, 2)!="do")
                                        	{
                                            	TransfInArb(F, cod, st, x+1);
                                            	st=x+2;
                                            	i=st-1;
                                        	}
                                        	else
                                        	{
                                            	TransfInArb(F, cod, st, i);
                                            	st=i+1;
                                        	}
                                    	}
                                    	else if(cod.substr(i+1,4) == "else")
                                    	{
                                        	--nrIfElse;
                                        	if(nrIfElse==0 && !OKif)///este un else de pe acest nivel
                                        	{
                                            	OKif=1;
                                            	TransfInArb(F, cod, st, i);
                                            	st=i+1;
                                        	}
                                    	}
                                    	else
                                    	{
                                        	nrIfElse=0;
                                        	OKif=0;
                                        	TransfInArb(F, cod, st, i);
                                        	st=i+1;
                                    	}
                                	}
                                	break;
                            	case ';':
                                	if(nrp==0 && nrP==0) /// probabil o instuctiune simpla
                                	{
                                    	if(cod.substr(i+1, 4)=="else" && !OKif)
                                    	{
                                        	--nrIfElse;
                                        	if(nrIfElse==0 && !OKif)
                                        	{
                                            	OKif=1;
                                            	TransfInArb(F, cod, st, i);
                                            	st=i+1;
                                        	}
                                    	}
                                    	else
                                    	{
                                        	nrIfElse=0;
                                        	OKif=0;
                                        	TransfInArb(F, cod, st, i);
                                        	st=i+1;
                                    	}
                                	}
                                	break;
                            	}//sf switch
                	}//sf for
            	}//sf else
        	}//sf if acolade
        	else if(cod[st]==';')
        	{
            	///este ; de dupa while din do_while
            	///nu ne trebuie
        	}
        	else///avem o singura instructiune
        	{
            	int sf=st;
            	int nrp=0;///(
            	int nrP=0;///{
            	bool OKg=0;///"
            	bool OKa=0;///'
            	int nrIfElse=0;///contor if+ else-
            	bool OKif=0;
            	bool ok=1;
            	while(ok)
            	{
                	if(!OKa || cod[sf]==39)
                    	if(!OKg || cod[sf]=='"')
                        	switch(cod[sf])
                        	{
                        	case 'i':
                            	if(cod.substr(sf, 3)=="if(" && nrP==0)
                                	++nrIfElse;
                            	break;
                        	case '"':
                            	OKg=!OKg;
                            	break;
                        	case 39:
                            	OKa=!OKa;
                            	break;
                        	case '(':
                            	++nrp;
                            	break;
                        	case ')':
                            	--nrp;
                            	break;
                        	case '{':
                            	++nrP;
                            	break;
                        	case '}':
                            	--nrP;
                            	if(nrP==0 && cod[sf+1]!=';')///sf de acolada de for/while/if
                            	{
                                	if(cod.substr(sf+1, 4)=="else" && sf<dr)
                                	{
                                    	--nrIfElse;
                                    	if(nrIfElse==0 && !OKif)
                                    	{
                                        	OKif=1;
                                        	TransfInArb(F, cod, st, sf);
                                        	st=sf+1;
                                    	}
                                	}
                                	else if(cod.substr(st, 2)=="do")
                                	{
                                    	TransfInArb(F, cod, st, sf);
                                    	st=sf+1;
                                	}
                                	else
                                    	ok=0;
                            	}
                            	break;
                        	case ';':
                            	if(nrp==0 && nrP==0)
                            	{
                                	if(cod.substr(sf+1, 4)=="else" && sf<dr)
                                	{
                                    	--nrIfElse;
                                    	if(nrIfElse==0 && !OKif)
                                    	{
                                        	OKif=1;
                                        	TransfInArb(F, cod, st, sf);
                                        	st=sf+1;
                                    	}
                                	}
                                	else
                                    	ok=0;
                            	}
                            	break;
                        	}
                	++sf;
            	}
            	--sf;
            	TransfInArb(F, cod, st, sf);
        	}//sf else (o singura instr)
    	}// sf if(st<dr)
	}
}

arbore TransformareInArbore(string cod, string nume)
{
	arbore A = new nodA;
	A->instr=nume;
	A->tip=0;
	initLista(A->L);

	int l=cod.length();
	int nrp=0;///nr paranteze deschise
	int nrP=0;///nr acolade deschise
	bool OKg=0;///"
	bool OKa=0;///'
	int nrIfElse=0;///contor ce creste la if si scade la else
	bool OKif=0;
	int st=0;///inceputul urmatorului chunk

	///C1: pana la ;
	///C2: pana cand nr{ = nr}
	///C3: pana cand nr( = nr)

	///impartim secventa de cod in chunkuri
	for(int i=0; i<l; ++i)
	{
    	if(!OKa || cod[i]==39)
        	if(!OKg || cod[i]=='"')
            	switch(cod[i])
            	{
            	case 'i':
                	if(cod.substr(i, 3)=="if(" && nrP==0)
                    	++nrIfElse;
                	break;
            	case '"':
                	OKg=!OKg;
                	break;
            	case 39:
                	OKa=!OKa;
                	break;
            	case '(':
                	++nrp;
                	break;
            	case ')':
                	--nrp;
                	break;
            	case '{':
                	++nrP;
                	break;
            	case '}':
                	--nrP;
                	if(nrP==0 && cod[i+1]!=';')///sf de acolada de for/while/if
                	{
                    	if(cod.substr(i+1,4) == "else")
                    	{
                        	--nrIfElse;
                        	if(nrIfElse==0 &&!OKif)///este un else de pe acest nivel
                        	{
                            	OKif=1;
                            	TransfInArb(A, cod, st, i);
                            	st=i+1;
                        	}
                    	}
                    	else if(cod.substr(i+1, 6)=="while(")///primul pas
                    	{
                        	///acum cautam sa vedem daca dupa acel while
                        	int x=i+7;
                        	while(cod[x]!=')') ++x;
                        	///daca dupa ) avem ; inseamna ca avem un do while
                        	///dar daca nu incepem cu el chunkul atunci este singura instructiune
                        	///a unei instructiuni fara acolade
                        	///deci trebuie sa luam cu tot cu while
                        	if(cod[x+1]==';' && cod.substr(st, 2)!="do")
                        	{
                            	/**
                            	for(i=1; i<=n; ++i)
                            	do{
                             	a++;
                            	}
                            	while(a);
                            	**/
                            	TransfInArb(A, cod, st, x+1);
                            	st=x+2;
                            	i=st-1;
                        	}
                        	else
                        	{
                            	///avem un while normal sau un do while normal
                            	TransfInArb(A, cod, st, i);
                            	st=i+1;
                        	}
                    	}
                    	else
                    	{
                        	OKif=0;
                        	nrIfElse=0;
                        	///nu urmeaza un while
                        	TransfInArb(A, cod, st, i);
                        	st=i+1;
                    	}
                	}
                	break;
            	case ';':
                	if(nrp==0 && nrP==0) /// instructiune simpla în afara parantezelor
                	{
                    	if(cod.substr(i+1, 4)=="else")
                    	{
                        	--nrIfElse;
                        	if(nrIfElse==0 && !OKif)
                        	{
                            	OKif=1;
                            	TransfInArb(A, cod, st, i);
                            	st=i+1;
                        	}
                    	}
                    	else
                    	{
                        	nrIfElse=0;
                        	OKif=0;
                        	TransfInArb(A, cod, st, i);
                        	st=i+1;
                    	}
                	}
                	break;
            	}//sf switch
	}//sf for
	return A;
}

int despartireInFunctii(arbore A[50], string &cod)
{
	int nrFunctii=-1;

	int l=cod.length();

	int lastGlobal=-1;
	int nrp=0, nrP=0;
	bool OKg=0;
	bool OKa=0;
	int st=0;
	int i;

	for(i=0; i<l; ++i)
	{
    	if(!OKa || cod[i]==39)
        	if(!OKg || cod[i]=='"')
            	switch(cod[i])
            	{
            	case 39:
                	OKa=!OKa;
                	break;
            	case '"':
                	OKg=!OKg;
                	break;
            	case '(':
                	++nrp;
                	break;
            	case ')':
                	--nrp;
                	break;
            	case '{':
                	++nrP;
                	break;
            	case '}':
                	--nrP;
                	if(nrP==0 && cod[i+1]!=';')///sf de acolada de functie
                	{
                    	///e sfarsitul unei functii

                    	///1 - introducem toate atribuirile globale in vectorul de arbori
                    	if(lastGlobal!=-1)
                    	{
                        	A[++nrFunctii]=TransformareInArbore(cod.substr(lastGlobal, (st-1)-lastGlobal+1), "global");
                        	lastGlobal=-1;///am terminat cu atribuirile
                    	}
                    	///2 - introducem functia in vector
                    	///gasim numele ei
                    	string numeFunctie;
                    	int sfNume=st;
                    	while(cod[sfNume]!='(') ++sfNume;
                    	--sfNume;///scapam de paranteza
                    	numeFunctie=cod.substr(st, sfNume-st+1);

                    	for(int j=0; j<nrCuvinteDictionar; j++)
                        	if(dictionar[j]==numeFunctie.substr(0, dictionar[j].length()))
                            	numeFunctie.insert(dictionar[j].length(), " ");

                    	///gasim inceputul
                    	int incFct=sfNume+1;
                    	while(cod[incFct]!='{') ++incFct;
                    	++incFct;///scapam de acolada
                    	///cream arborele functiei
                    	A[++nrFunctii]=TransformareInArbore(cod.substr(incFct, (i-1)-incFct+1), numeFunctie);
                    	st=i+1;
                	}
                	break;
            	case ';':
                	///am dat de o atribuire globala
                	if(nrp==0 && nrP==0)
                	{
                    	if(cod[st-1]=='}' || st==0)
                        	lastGlobal=st;
                    	st=i+1;
                	}
                	break;
            	}//sf switch
	}//sf for

	if(lastGlobal!=-1)
    	A[++nrFunctii]=TransformareInArbore(cod.substr(lastGlobal, (l-1)-lastGlobal+1), "global");

	return nrFunctii;
}

struct punct
{
	int x, y;
};

/*
1-instr
2-cond
3-if
4-else
5-while
6-for
7-do
8-break
9-continue
10-switch
11-case
*/

void toChar(char nume[256], int nrc, string str)
{
	///transferam stringul in vectorul de char
	strcpy(nume, "");
	for(int i=0; i<nrc; ++i)
    	nume[i]=str[i];
	nume[nrc]='\0';
}

int nrLinii(nodL *L)
{
	arbore A=L->fiu;
	if (A->tip==2)
    	return 0;///conditia din paranteze nu se ia in considerare
	else if(A->tip==1)
    	return 1;///instructiunea nu au fii, cazuri de baza, este nevoie de o linie

	int nr=0;
	nodL *p= A->L.prim;

	if(A->tip==11 && A->instr=="case") /// este case
    	p=p->urm; /// trecem peste conditie
	if(A->tip==10) /// este switch
	{
    	int maxLinii=0;
    	int nrL=0;
    	p=p->urm;/// trecem peste conditie
    	int cont=1; /// pentru a tine evidenta blocurilor
    	while(p!=NULL)
    	{
        	if(p->fiu->instr=="default")
            	cont--;
        	nrL=cont+nrLinii(p);
        	if(nrL>maxLinii)
            	maxLinii=nrL;
        	cont++;

        	p=p->urm;

    	}
    	return maxLinii;
	}
	else if(A->tip==3) /// este if
	{
    	int ifMax=0, elseMax=0;

    	while(p!=NULL)
    	{
        	if(p->fiu->tip!=4)
            	ifMax+=nrLinii(p);

        	if(p->fiu->tip==7) /// este insturctiunea "do" sau "if"
            	p=p->urm;

        	p=p->urm;
    	}

    	if(L->urm!=NULL)
        	if(L->urm->fiu->tip==4)
        	{
            	p=L->urm->fiu->L.prim;

            	while(p!=NULL)
            	{
                	if(p->fiu->tip!=4)
                    	elseMax+=nrLinii(p);

                	if(p->fiu->tip==7) /// este insturctiunea "do" sau "if"
                    	p=p->urm;
                	p=p->urm;
            	}
        	}

    	return 1+max(ifMax,elseMax);
	}
	else
	{
    	while(p!=NULL)
    	{
        	if(p->fiu->tip!=4)
            	nr+=nrLinii(p);

        	if(p->fiu->tip==7) /// este insturctiunea "do" sau "if"
            	p=p->urm;
        	p=p->urm;
    	}

    	if(A->tip==0 || A->tip==11)
        	return nr;
    	else
        	return nr+1;
	}
}

int latimeArbore(nodL *nl, int lineH, int spatiuSt)
{
	arbore A=nl->fiu;
	if(A==NULL)
    	return 999;

	int maxx;
	nodL *i= A->L.prim;
	char nume[256];

	///PAS 1: INITIALIZAM MAXIMUL

	////cond/switch/case
	if(A->tip==0 || A->tip==10 || A->tip==11)
    	maxx=0;
	///instr
	if(A->tip==1 || A->tip==2)
	{
    	///initializam cu lungimea instructiunii
    	int nrc=A->instr.length();
    	toChar(nume, nrc, A->instr);
    	maxx = textwidth(nume);
	}
	///if/else/while/for
	if(A->tip>=3 && A->tip<=6)
	{
    	///initializam maximul cu latimea conditiei
    	arbore cond = A->L.prim->fiu;
    	int nrc=cond->instr.length();
    	toChar(nume, nrc, cond->instr);
    	maxx = textwidth(nume);
    	if(A->tip==3) maxx*=4;
    	i=i->urm;
	}
	///do while
	if(A->tip==7)
	{
    	/// coditia de la do este in fiul nodului ce urmeaza dupa do
    	arbore cond = nl->urm->fiu->L.prim->fiu;
    	int nrc=cond->instr.length();
    	toChar(nume, nrc, cond->instr);
    	maxx = textwidth(nume);
	}
	///break, continue
	if(A->tip==8)
    	maxx=textwidth("break;");
	if(A->tip==9)
    	maxx=textwidth("continue;");
	///default
	if(A->tip==11 && A->instr=="default")
    	maxx=textwidth("default");

	maxx+=2*spatiuSt;

	int l;///latimea unui fiu
	/// PAS 2: COMPARAM MAXIMUL CU MAXIMUL FIILOR
	if(A->tip==0 || A->tip==11)
	{
    	while(i!=NULL)
    	{
        	if(i->fiu->tip!=4)
        	{
            	l=latimeArbore(i, lineH, spatiuSt);
            	if(maxx<l)
                	maxx=l;
        	}

        	if(i->fiu->tip==7) /// trecem peste while de dupa do
            	i=i->urm;
        	i=i->urm;
    	}
	}
	else if(A->tip==10)
	{
    	i=i->urm;
    	while(i!=NULL)
    	{
        	l=latimeArbore(i, lineH, spatiuSt);
        	if(l>maxx)
            	maxx=l;
        	i=i->urm;
    	}

    	if(A->L.ultim->fiu->instr!="default")
        	maxx=maxx*(A->L.lg);
    	else
        	maxx=maxx*(A->L.lg-1);
	}
	else if(A->tip>=5 && A->tip<=7) /// while, for, do
	{
    	while(i!=NULL)
    	{
        	if(i->fiu->tip!=4)
        	{
            	l=latimeArbore(i, lineH, spatiuSt);
            	if(maxx<l+spatiuSt)
                	maxx=l+spatiuSt;
        	}

        	if(i->fiu->tip==7) /// trecem peste else si (do) while
            	i=i->urm;
        	i=i->urm;
    	}
	}
	else if(A->tip==3) /// este if
	{
    	int ifMax=spatiuSt;
    	while(i!=NULL)
    	{
        	if(i->fiu->tip!=4)
        	{
            	l=latimeArbore(i, lineH, spatiuSt);
            	if(ifMax<l)
                	ifMax=l;
        	}

        	if(i->fiu->tip==7)
            	i=i->urm;
        	i=i->urm;
    	}

    	nodL *posibilElse = nl->urm;
    	int elseMax;
    	if(posibilElse!= NULL)
    	{
        	if(posibilElse->fiu->tip==4)
        	{
            	i=posibilElse->fiu->L.prim;
            	elseMax=spatiuSt;
            	while(i!=NULL)
            	{
                	if(i->fiu->tip!=4)
                	{
                    	l=latimeArbore(i, lineH, spatiuSt);
                    	if(elseMax<l)
                        	elseMax=l;
                	}

                	if(i->fiu->tip==7)
                    	i=i->urm;
                	i=i->urm;
            	}
        	}
        	else
            	elseMax=spatiuSt;
    	}
    	else
        	elseMax=spatiuSt;

    	if(maxx<ifMax+elseMax)
        	maxx=ifMax+elseMax;
	}

	///Debugging
	//cout<<spatiuSt<<' '<<A->instr<<' '<<maxx<<'\n';
	return maxx;
}

///instr
void deseneaza1(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)
{
	//line(x1, y1+lineH, x2, y1+lineH);
	char nume[256]; int nrc=A->instr.length();
	toChar(nume, nrc, A->instr);
	outtextxy(x1+spatiuSt, y1+lineH/2,  nume);
}

///if si else
int deseneaza3(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)
{
	nodL *p= A->L.prim->urm;

	int latMax=latimeArbore(p,  lineH, spatiuSt);
	p=p->urm;
	int lat;
	while(p!=NULL)
	{
    	lat=latimeArbore(p, lineH, spatiuSt);
    	if(lat>latMax)
        	latMax=lat;
    	if(p->fiu->tip==7)
        	p=p->urm;
    	p=p->urm;
    	if(p!=NULL)
        	if(p->fiu->tip==4)
            	p=p->urm;
	}  /// calculeaza latimea necesara instuctiunilor din if

	line(x1, y1+lineH, x2, y1+lineH);
	line(x1, y1, x1+latMax, y1 + lineH);
	line(x1+latMax, y1 + lineH, x2, y1);

	int nrc=A->L.prim->fiu->instr.length();
	char nume[256];
	toChar(nume, nrc, A->L.prim->fiu->instr);
	int lungNume=textwidth(nume);

	outtextxy(x1+latMax-lungNume/2, y1+lineH/4,  nume);
	outtextxy(x1+latMax/4-5, y1+lineH/2,  "yes");
	outtextxy(x2-(x2-x1-latMax)/4-textwidth("no")/2, y1+lineH/2,  "no");

	return latMax;
}

///while
void deseneaza5(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)
{
	int nrc=A->L.prim->fiu->instr.length();
	char nume[256];
	toChar(nume, nrc, A->L.prim->fiu->instr);
	outtextxy(x1+spatiuSt, y1+lineH/2,  nume);
	line(x1+spatiuSt, y1+lineH, x2, y1+lineH);
	line(x1+spatiuSt, y1+lineH, x1+spatiuSt, y2);
}

///for
void deseneaza6(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)
{
	int nrc=A->L.prim->fiu->instr.length();
	char nume[256];
	toChar(nume, nrc, A->L.prim->fiu->instr);
	outtextxy(x1+spatiuSt, y1+lineH/2,  nume);
	line(x1+spatiuSt, y1+lineH, x2, y1+lineH);
	line(x1+spatiuSt, y1+lineH, x1+spatiuSt, y2);
}

///do while
void deseneaza7(nodL* p, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)
{
	int nrc=p->urm->fiu->L.prim->fiu->instr.length();
	char nume[256];
	toChar(nume, nrc, p->urm->fiu->L.prim->fiu->instr);
	outtextxy(x1+spatiuSt, y2-spatiuSt,  nume);

	///  |
	/// 1|__
	///   2

	line(x1+spatiuSt, y2-lineH, x2, y2-lineH);   ///1
	line(x1+spatiuSt, y2-lineH, x1+spatiuSt, y1); ///2
}

///break
void deseneaza8(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)/// break;
{
	line(x1, y1+lineH, x2, y1+lineH);
	line(x1, y1+lineH/2, x1+lineH/4, y1);
	line(x1, y1+lineH/2, x1+lineH/4, y2);

	int nrc=A->instr.length();
	char nume[256];
	toChar(nume, nrc, A->instr);
	outtextxy(x1+spatiuSt, y1+lineH/2-textheight(nume)/2,  nume);
}

///continue;
void deseneaza9(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt)/// continue;
{
	line(x1, y1+lineH, x2, y1+lineH);
	line(x1, y1+lineH/2, x1+lineH/4, y1);
	line(x1, y1+lineH/2, x1+lineH/4, y2);

	int nrc=A->instr.length();
	char nume[256]="";
	for(int i=0; i<nrc; ++i)
    	nume[i]=A->instr[i];
	nume[nrc]='\0';
	outtextxy(x1+spatiuSt, y1+lineH/2-textheight(nume)/2,  nume);
}

///switch
void deseneaza10(arbore &A, int x1, int y1, int x2, int y2, int lineH, int spatiuSt, int maxlat)/// switch si case
{

	int xv=x1; /// intializam coordonatele varfului triunghiului
	int yv=y1;
	nodL *p= A->L.prim; /// accesam primul fiu al lui switch care este conditia

	p=p->urm; /// accesam primul case

	while(p!=NULL)
	{
    	if(p->fiu->instr=="default")
        	break;

    	line(xv, yv, xv, yv+lineH); /// cateta mica a triunghiului
    	line(xv, yv, xv+maxlat, yv+lineH); /// ipotenuza triunghiului
    	line(xv, yv+lineH, xv+maxlat, yv+lineH);

    	int nrc=p->fiu->L.prim->fiu->instr.length();
    	char nume[256];
    	toChar(nume, nrc, p->fiu->L.prim->fiu->instr);
    	outtextxy(xv+5, yv+lineH-30, nume);

    	xv= xv+ maxlat;
    	yv=yv+lineH;

    	p=p->urm;
	}

	p=A->L.prim;
	int nrc=p->fiu->instr.length(); /// desenam conditia din switch
	char nume[256];
	toChar(nume, nrc, p->fiu->instr);
	outtextxy(xv-10, (y1+yv)/2, nume);

	line(xv, yv, x2, y1);
	line(xv, yv, x2, yv);

	/// desenam default
	p=A->L.ultim;
	if(p->fiu->instr=="default")
	{
    	char nume[256]="default";
    	outtextxy(x2-textwidth(nume)-10, yv-30, nume);
	}
}

int nrLiniiNecIfSauElse(nodL *i)
{
	int nrlNec=0;
	while(i!=NULL)
	{
    	nrlNec+=nrLinii(i);

    	if(i->fiu->tip==7)
        	i=i->urm;
    	i=i->urm;
    	if(i!=NULL)
        	if(i->fiu->tip==4)
            	i=i->urm;
	}
	return nrlNec;
}

void deseneaza(nodL *L, int lineH, int spatiuSt, int x1, int y1, int x2, int y2)
{
	int l;
	int maxlat=0;
	arbore A=L->fiu;
	if(A==NULL)
    	return;

	nodL *i = A->L.prim;
	if(A->tip==10)

	{
    	i=i->urm;/// trecem peste conditia din switch
    	int lat;
    	while(i!=NULL)
    	{
        	lat=latimeArbore(i,  lineH, spatiuSt);
        	if(lat>maxlat)
            	maxlat=lat;
        	i=i->urm;
    	}
    	i=A->L.prim->urm;
	}

	///deseneaza conturul diagramei
	///   ___1__
	///  | 	   |
	/// 2| 	   |3
	///  |_____|
	///     4
	line(x1, y1, x2, y1);/// 1
	line(x1, y1, x1, y2);/// 2
	line(x2, y1, x2, y2);/// 3
	line(x1, y2, x2, y2);/// 4

	switch(A->tip)
	{
	case 1:
    	deseneaza1(A, x1, y1, x2, y2, lineH, spatiuSt); /// instructiune simpla
    	break;
	case 2:
    	///conditia este folosita in desenarea instructiunilor
    	break;
	case 3:
    	l=deseneaza3(A, x1, y1, x2, y2, lineH, spatiuSt); /// if/else
    	break;
	case 4:
    	///else este folosit in desenarea if
    	break;
	case 5:
    	deseneaza5(A, x1, y1, x2, y2, lineH, spatiuSt);/// while
    	break;
	case 6:
    	deseneaza6(A, x1, y1, x2, y2, lineH, spatiuSt); /// for
    	break;
	case 7:
    	deseneaza7(L, x1, y1, x2, y2, lineH, spatiuSt); /// do while
    	break;
	case 8:
    	deseneaza8(A, x1, y1, x2, y2, lineH, spatiuSt); /// break
    	break;
	case 9:
    	deseneaza9(A, x1, y1, x2, y2, lineH, spatiuSt); /// continue
    	break;
	case 10:
    	deseneaza10(A, x1, y1, x2, y2, lineH, spatiuSt, maxlat); /// switch si case
    	break;

	}


	int nrlNec;///numar linii necesare
	int xSt, ySt, xDr, yDr;

	///setam xSt
	xSt=x1;
	///indentarea pentru functiile predefinite
	if(A->tip == 5 || A->tip == 6 )
	{
    	i=i->urm;///dam skip la conditie (while , for)
    	xSt+=spatiuSt;
	}
	else if(A->tip==11 && A->instr=="case")
	{
    	i=i->urm;
	}
	else if(A->tip==7)
	{
    	///aici nu avem dconditie ca fiu al lui do
    	xSt+=spatiuSt;
	}
	///setam ySt si yDr
	if(A->tip == 0 || A->tip == 7 || A->tip==11)
    	ySt=yDr=y1;
	else if(A->tip == 10)
	{
    	ySt=y1+lineH;
    	yDr=y2;
	}
	else
    	ySt=yDr=y1+lineH;
	///setam xDr
	if(A->tip==3)
    	xDr=x1+l;
	else if(A->tip==10)
    	xDr=x1;
	else
    	xDr=x2;

	///calculam inaltimea necesara la if
	int newLineH, nrlIf;
	if(A->tip==3)
	{
    	nrlIf=nrLiniiNecIfSauElse(i);
    	newLineH=(y2-y1-lineH)/nrlIf;
	}

	///desenam ce este in interiourul functiei predefinite
	if(A->tip==10)
	{
    	///numarul de linii Switch
    	int nrLSwitch=nrLinii(L)-1; /// numarul maxim de linii din Switch

    	while(i!=NULL)
    	{
        	if(i->fiu->instr=="default")
            	break;

        	xDr=xDr+maxlat;

        	int newLineH; /// avem nevoie de un new lineH pentru a distribui uniform instructiunile din case in dreptunghi
        	int nrLCase= nrLinii(i); /// facem numarul de intructuini din case
        	newLineH=lineH*nrLSwitch/nrLCase;
        	nrLSwitch--; /// scade pentru fiecare case

        	deseneaza(i, newLineH, spatiuSt, xSt, ySt, xDr, yDr);
        	xSt=xDr;
        	ySt=ySt+lineH;
        	i=i->urm;
    	}
    	if(i!=NULL) /// exista default
    	{
        	ySt-=lineH;
        	xSt=xDr;
        	xDr=x2;

        	int newLineH; /// avem nevoie de un new lineH pentru a distribui uniform instructiunile din case in dreptunghi
        	int nrLCase= nrLinii(i); /// facem numarul de intructuini din case
        	nrLSwitch++;
        	newLineH=lineH*nrLSwitch/nrLCase;

        	deseneaza(i, newLineH, spatiuSt, xSt, ySt, xDr, yDr);
    	}
	}
	else
	{
    	while(i!=NULL)
    	{
        	///nr linii necesare pentru blocul acesta de instructiuni
        	///adica putem avea un while in while care are nevoie de 3 linii
        	///deci vom pune yDr cu 3 linii mai jos
        	nrlNec = nrLinii(i);
        	if(A->tip!=3)
        	{
            	yDr += nrlNec*lineH;
            	deseneaza(i, lineH, spatiuSt, xSt, ySt, xDr, yDr);
        	}
        	else
        	{
            	yDr += nrlNec*newLineH;
            	deseneaza(i, newLineH, spatiuSt, xSt, ySt, xDr, yDr);
        	}

        	ySt = yDr;

        	///daca e do sarim peste while la desenare
        	if(i->fiu->tip==7)
            	i=i->urm;
        	i=i->urm;
        	if(i!=NULL)
            	if(i->fiu->tip==4)
                	i=i->urm;
    	}

    	if(A->tip==3)
        	if(L->urm!=NULL)
            	if(L->urm->fiu->tip==4)
            	{
                	i=L->urm->fiu->L.prim;
                	nrlIf=nrLiniiNecIfSauElse(i);
                	newLineH=(y2-y1-lineH)/nrlIf;
                	xSt=xDr;
                	xDr=x2;
                	ySt=yDr=y1+ lineH;
                	while(i!=NULL)
                	{
                    	nrlNec = nrLinii(i);
                    	yDr += nrlNec*newLineH;

                    	deseneaza(i, newLineH, spatiuSt, xSt, ySt, xDr, yDr);

                    	ySt = yDr;

                    	if(i->fiu->tip==7)
                        	i=i->urm;
                    	i=i->urm;
                    	if(i!=NULL)
                        	if(i->fiu->tip==4)
                            	i=i->urm;
                	}
            	}

	}
}

int scrW=getmaxwidth();
int scrH=getmaxheight();

void scrollBar(int l, int scrL, int cont, int lineH)
{
	///acoperim diagrama
	setfillstyle(SOLID_FILL, BLACK);
	bar(scrW-3*scrW/100,
    	0,
    	scrW,
    	scrH);
	///bara alba
	setfillstyle(SOLID_FILL, WHITE);
	bar(scrW-2*scrW/100,
    	10,
    	scrW-scrW/100,
    	scrH-10);
	///bara gri
	setfillstyle(SOLID_FILL, BLUE);
	bar(scrW-2*scrW/100,
    	10+(scrH-20)*cont/(2*l),
    	scrW-scrW/100,
    	10+(scrH-20)*(cont+2*scrL)/(2*l));
	///reset
	setfillstyle(SOLID_FILL, WHITE);
}

void scrollCircle(int fctCurenta, int nrFunctii)
{
	///acoperim diagrama
	setfillstyle(SOLID_FILL, BLACK);
	bar(0,
    	0,
    	scrH/20+scrW/50,
    	scrH);
	setfillstyle(SOLID_FILL, WHITE);

	///calculam raza si coordonatele primului cerc
	int raza;
	raza=min(scrH/40,(scrH-30-textheight("l"))/(4*nrFunctii+2));
	int x, y;
	x=scrH/40+scrW/100;
	y=20+textheight("l")+raza;

	///desenam toate cercurile
	for(int i=0; i<=nrFunctii; i++)
	{
    	if(i==fctCurenta)
    	{
        	setfillstyle(SOLID_FILL, BLUE);
        	fillellipse(x , y, raza, raza);
        	setfillstyle(SOLID_FILL, WHITE);
    	}
    	else
        	fillellipse(x , y, raza, raza);
    	y=y+4*raza;
	}
}

void deseneazaDiagrama(arbore A, int cont1, int &cont2, int l, int scrL, int fctCurenta, int nrFunctii)
{
	nodL *L = new nodL;
	L->fiu=A;
	L->urm=NULL;

	int lineH=(scrH-20)/scrL;
	int spatiuSt=lineH/2;

	int maxim=latimeArbore(L, lineH, spatiuSt);
	int x1, x2, y1, y2;

	/// conturul schemei
	x1=(scrW-maxim)/2;
	if(x1<scrH/20+scrW/50) x1=scrH/20+scrW/50;
	x1-=cont2*scrW/30;
	x2=x1+maxim;
	if(x1<scrH/20+scrW/50 && x2<scrW-3*scrW/100)
	{
    	x1+=scrW/30;
    	x2=scrW-3*scrW/100;
    	--cont2;
	}

	y1=10 - cont1*lineH/2;
	y2=10+l*lineH;

	bar(x1-1, y1-1, x2+1, y2+1);
	setcolor(BLACK);
	setbkcolor(WHITE);

	deseneaza(L, lineH, spatiuSt, x1, y1, x2, y2);
	///deseneaza scrollbarul
	scrollBar(l, scrL, cont1, lineH);
	///deseneaza cercurile
	scrollCircle(fctCurenta, nrFunctii);

	/// desenam numele functiei
	int nrc=A->instr.length();
	char nume[256];
	toChar(nume, nrc, A->instr);

	setcolor(WHITE);
	setbkcolor(BLUE);
	outtextxy(10, 10, nume);
}

void deseneazaTot(arbore A[], int nrFunctii)
{
	int l[50], scrL[50]; /// vectori pentru nrLinii pt fiecare Arbore(functie desenata)
	initwindow(scrW, scrH);

	for(int i=0; i<=nrFunctii; i++)
	{
    	nodL *L = new nodL;
    	L->fiu=A[i];
    	L->urm=NULL;

    	l[i]=nrLinii(L);
    	scrL[i]=min(15, l[i]);
	}
	int fctCurenta=0;
	int cont1=0;
	int cont2=0;
	char key=' ';
	while(key!='q')
	{
    	setfillstyle(SOLID_FILL, BLACK);
    	bar(0, 0, scrW, scrH);
    	setfillstyle(SOLID_FILL, WHITE);

    	deseneazaDiagrama(A[fctCurenta], cont1, cont2, l[fctCurenta], scrL[fctCurenta], fctCurenta, nrFunctii);

    	bool ok=0;
    	while(true)
    	{
        	key=getch();
        	switch(key)
        	{
        	case 'w':
            	if(cont1>0)
            	{
                	--cont1;
                	ok=1;
            	}
            	break;
        	case 's':
            	if(cont1<2*(l[fctCurenta]-scrL[fctCurenta]))
            	{
                	++cont1;
                	ok=1;
            	}
            	break;
        	case 'q':
            	ok=1;
            	break;
        	case 'e':
            	if(fctCurenta>0)
            	{
                	fctCurenta--;
                	cont1=0;
                	cont2=0;
                	ok=1;
            	}
            	break;
        	case 'd':
            	if(fctCurenta<nrFunctii)
            	{
                	fctCurenta++;
                	cont1=0;
                	cont2=0;
                	ok=1;
            	}
            	break;
        	case 'a':
            	if(cont2>0)
            	{
                	--cont2;
                	ok=1;
            	}
            	break;
        	case 'f':
            	++cont2;
            	ok=1;
            	break;
        	}

        	if(ok)
            	break;
    	}
	}

	closegraph();
}

void afisArbore(arbore A[50], int nrFunctii);

int main()
{
	string cod="";
	arbore A[50];

	citire(cod, 1);

	///Debugging pentru despartirea in chunkuri
	/*
	for(int i=0; (unsigned)i<cod.length(); ++i)
    	cout<<i%10;
	cout<<'\n'<<cod<<'\n';
	*/

	int n=despartireInFunctii(A, cod);
	deseneazaTot(A, n);
	//afisArbore(A, n);

	/*  do{
 		 cout<<"Pentru desenarea diagramei din primul fisier apasati tasta '1'"<<endl;
 		 cout<<"Pentru desenarea diagramei din al doilea fisier apasati tasta '2'"<<endl;
 		 cout<<"Pentru desenarea diagramei din al treilea fisier apasati tasta '3'"<<endl;
 		 cout<<"Pentru desenarea diagramei din al patrulea fisier apasati tasta '4'"<<endl;

  		 cin>>x;

 		 switch(x)
 		 {
 		 case 1:
     		 citire(cod, 1);

     		 break;
 		 case 2:
     		 citire(cod, 2);

     		 break;
 		 case 3:
     		 citire(cod, 3);
     		 break;
 		 case 4:
     		 citire(cod, 4);
     		 break;
 		 }

 	int n=despartireInFunctii(A, cod);
  	deseneazaTot(A, n);


	}while(x!=0);
	*/
}

///V
int inaltime(arbore A)
{
	if(A==NULL)
    	return 0;
	else if(A->L.lg==0)///nu are fii
    	return 1;
	else///aceaasi functie ca la logica
	{
    	int height;
    	///initializam cu inaltimea subarborelui primului fiu
    	int maxx=inaltime(A->L.prim->fiu);

    	///incepem algoritmu de maxim de la al doilea fiu
    	nodL *i = A->L.prim->urm;
    	while(i!=NULL)
    	{
        	height=inaltime(i->fiu);
        	if(maxx<height)
            	maxx=height;

        	///trecem la urmatorul fiu
        	i=i->urm;
    	}

    	///returnam 1(nodul nostru) + maximul inaltimii subarborilor
    	return 1+maxx;
	}
}

void nrNoduriLvl(arbore A, int nivel[100], int lvl)
{
	if(A==NULL)
    	return;
	else
	{
    	///incrementam contorul de noduri pe nivelul curent
    	nivel[lvl]++; //cout<<lvl<<' '<<nivel[lvl]<<'\n';

    	nodL *i = A->L.prim;
    	while(i!=NULL)
    	{
        	///adaugam fiii la v[]
        	nrNoduriLvl(i->fiu, nivel, lvl+1);

        	///trecem la urmatorul fiu
        	i=i->urm;
    	}
	}
}

void afisArb(arbore A, int x, int y, int lvl, int lvlH, int inc[100], int v[100])
{
	///scriem instr
	int nrc=A->instr.length(); char nume[256];
	toChar(nume, nrc, A->instr);
	outtextxy(x-textwidth(nume)/2, y+2, nume);

	///facem dreptunghiul
	punct B, D;
	B.y=y;
	B.x=x-textwidth(nume)/2-2;
	D.y=y+20;
	D.x=x+textwidth(nume)/2+2;
	line(B.x, B.y, D.x, B.y);
	line(D.x, B.y, D.x, D.y);
	line(D.x, D.y, B.x, D.y);
	line(B.x, D.y, B.x, B.y);

	///procesam fiii
	nodL *i = A->L.prim;
	int fiuX, fiuY;
	while(i!=NULL)
	{
    	fiuX=inc[lvl+1];
    	fiuY=y+lvlH;

    	line(x, y+20, fiuX, fiuY);
    	afisArb(i->fiu, fiuX, fiuY, lvl+1, lvlH, inc, v);

    	inc[lvl+1] += v[lvl+1];
    	i=i->urm;
	}
}

void afisArbore(arbore A[50], int nrFunctii)
{
	initwindow(scrW, scrH);

	int cont=0;
	char key=' ';
	while(key!='q')
	{
    	///stergem ce era inainte
    	setfillstyle(SOLID_FILL, BLACK);
    	bar(0, 0, scrW, scrH);
    	setfillstyle(SOLID_FILL, WHITE);

    	///calculam inaltimea arborelui si numarul de noduri pe fiecare nivel
    	int height = inaltime(A[cont]);
    	int v[100], i;
    	for(i=1; i<=height; ++i)
        	v[i]=0;
    	nrNoduriLvl(A[cont], v, 1);

    	///calculam inaltimea unui nivel si distanta dintre noduri la fiecare nivel
    	int lvlH;
    	if(height==1) lvlH=2*scrH;
    	else lvlH=(4*scrH/5)/(height-1);///80%din ecran impartit in n-1 parti
    	for(i=1; i<=height; ++i)
        	v[i]=scrW/v[i];

    	///initializam vectorul unde stocam inceputul din stg al fiecarui nivel
    	int inc[100];
    	for(i=1; i<=height; ++i)
        	inc[i]=v[i]/2;

    	///afisam
    	afisArb(A[cont], inc[1], scrH/10, 1, lvlH, inc, v);

    	key=getch();
    	switch(key)
    	{
    	case 'w':
        	if(cont>0)
            	--cont;
        	break;
    	case 's':
        	if(cont<nrFunctii)
            	++cont;
        	break;
    	}
	}

	closegraph();
}
///V
