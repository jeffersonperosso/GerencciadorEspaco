#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Caixa
{
	int info;
	struct Caixa *prox;
};
typedef struct Caixa caixa;
struct arquivo
{
	char nome[10];
	int t;
	struct arquivo *prox;
};
typedef struct arquivo Arquivo;
struct espaco
{
	
	struct espaco *prox;
	Arquivo *arq;
	int t,tInicial;
};
typedef struct espaco Espaco;
struct Fila
{
	Espaco *disco;
	struct Fila *prox;
};
typedef struct Fila fila;
int qtdetotal(Espaco *d)
{
	int qtde=0;
	while(d!=NULL)
	{
		qtde+=d->t;
		
		d=d->prox;
	}
	return qtde;
}
int qtdearquivo (Arquivo *arq)
{
	int qtde=0;
	while(arq!=NULL)
	{
		qtde+=arq->t;
		arq=arq->prox;
	}
	return qtde;
	
}
int conversor(char op[])
{
	int cap;
	char tipo[3];
	tipo[0]=op[strlen(op)-2];
	tipo[1]=op[strlen(op)-1];
	tipo[2]='\0';
	op[strlen(op)-2]='\0';
	cap=atoi(op);
	if(strcmpi(tipo,"KB")==0)
		return cap;
	if(strcmpi(tipo,"MB")==0)
		return cap*1024;
	if(strcmpi(tipo,"GB")==0)
		return cap*1024*1024;

}
void separar(char op[],char nome[],char tamanho[])
{
	char aux[50];
	int i=0,c=0;
	strcpy(aux,op);
	while(aux[i]!=' ')
	{
		op[i]=aux[i];
		i++;
	}
	op[i]='\0';
	i++;
	while(aux[i]!=' ' && aux[i]!='\0')
	{
		nome[c]=aux[i];
		i++;
		c++;	
	}
	nome[c]='\0';
	c=0;
	i++;
	if(aux[i]!='\0')
		while(aux[i]!='\0')
		{
			tamanho[c]=aux[i];
			i++;
			c++;
		}
		tamanho[c]='\0';
}
void tirarzero(Espaco *d)
{
	Arquivo *arq,*exc;
	while(d!=NULL)
	{
		if(d->arq!=NULL)
		{
			
			if(d->arq->t==0)
			{
				exc=d->arq;
				d->arq=d->arq->prox;
				free(exc);
			}
			if(d->arq!=NULL)
			{
				arq=d->arq;
				while(arq->prox!=NULL)
				{
					if(arq->prox->t==0)
					{
						exc=arq->prox;
						arq=exc->prox;
						free(exc);
					}	
						
					arq=arq->prox;
				}
			}
			
			
			
			
		}
		d=d->prox;
	}
}
void exibe(Espaco *d);
void otimiza(Espaco *d)
{
	
	Espaco *auxE, *inicio=d;
	Arquivo *auxA,*verigual;
	while(d!=NULL)
	{
		
		if(d->arq==NULL) //se n tiver arq nenhum, tras do bloco mais proximo
		{
			
			auxE=d->prox;
			while(auxE!=NULL && auxE->arq==NULL)
			{
				auxE=auxE->prox;
			}
			if(auxE!=NULL)
			{
				d->arq=auxE->arq;
				d->t=auxE->t;
				auxE->arq=NULL;
				auxE->t=auxE->tInicial;
			}
			else
				d=d->prox;
		}
		else
		{
			auxA=d->arq;
			while(d->t>0 && auxA!=NULL) //rodar disco
			{
				auxE=d->prox;
				while(auxE!=NULL)
				{
					if(auxE->arq!=NULL)
					{
						verigual=auxE->arq;
						while(verigual!=NULL)
						{
							if(strcmp(verigual->nome,auxA->nome)==0)
							{
								if(d->t>verigual->t)
								{
									d->t-=verigual->t;
									auxE->t+=verigual->t;
									auxA->t+=verigual->t;
									verigual->t=0;
								}
								else
								{
									verigual->t-=d->t;
									auxE->t+=d->t;
									auxA->t+=d->t;
									d->t=0;
								}
							}
							if(d->t==0)
								verigual=NULL;
							else
								verigual=verigual->prox;
						}	
					}
					auxE=auxE->prox;
				}
				auxA=auxA->prox;			
			}
			d=d->prox;
		}
		
	}
	tirarzero(inicio);
}
void verificarespaco(Espaco **d, int t)
{
	
	int qtde=0;
	Espaco *inicio=*d;
	Espaco *comeco=*d;
	
	while(*d!=NULL)
	{
		if((*d)->t>0)
		{
			qtde+=(*d)->t;
			*d=(*d)->prox;	
		}
		else
		{
			*d=(*d)->prox;
			inicio=*d;
			qtde=0;
		}
		if(qtde>=t)
			*d=NULL;
		
			
	}
	*d=comeco;
	if(qtde<t)
		otimiza(comeco);
	
		
	
		
}

char inserir(char nome[],int t,Espaco *d)
{
	char discocheio=false;
	Arquivo *arq;
	if(qtdetotal(d)>=t)
	{
		verificarespaco(&d,t);
		while(d!=NULL && t>0)
		{
			if(d->t>0)
			{
				if(d->arq==NULL)
				{
					d->arq=(arquivo*)malloc(sizeof(arquivo));
					strcpy(d->arq->nome,nome);
					d->arq->prox=NULL;
					if(t>d->t)
					{
						d->arq->t=d->t;
						t-=d->t;
						d->t=0;
					}	
					else
					{
						d->t-=t;
						d->arq->t=t;
						t=0;
					}
				}
				else
				{
					arq=d->arq;
					while(arq->prox!=NULL)
						arq=arq->prox;
					arq->prox=(arquivo*)malloc(sizeof(arquivo));
					strcpy(arq->prox->nome,nome);
					arq->prox->prox=NULL;
					if(t>d->t)
					{
						arq->prox->t=d->t;
						t-=d->t;
						d->t=0;
					}	
					else
					{
						d->t-=t;
						arq->prox->t=t;
						t=0;
					}
					
				}
			}
			d=d->prox;
		}
	}
	else
		discocheio=true;
	return discocheio;
}
void remover(char nome[],Espaco *d)
{
	Arquivo *exc, *aux;
	while(d!=NULL)
	{
		if(d->arq!=NULL)
		{
			if(strcmp(d->arq->nome,nome)==0)
			{
				d->t+=d->arq->t;
				exc=d->arq;
				d->arq=d->arq->prox;
				free(exc);
			}
			else
			if(d->arq->prox!=NULL)
			{
				aux=d->arq;
				exc=d->arq->prox;
				while(exc!=NULL)
				{
					if(strcmp(exc->nome,nome)==0)
					{
						d->t+=exc->t;
						aux->prox=exc->prox;
						free(exc);
						exc=NULL;
					}
					else
					{
						aux=exc;
						exc=exc->prox;	
					}
					
				}
				
			}
			
		}
		
		d=d->prox;
	}
}
void criardisco(Espaco **disco,int cap)
{
	*disco=(Espaco*)malloc(sizeof(Espaco));
	(*disco)->t=cap/8;
	(*disco)->tInicial=cap/8;
	(*disco)->arq=NULL;
	Espaco *aux=*disco;
	for(int i=1;i<8;i++)
	{
		aux->prox=(Espaco*)malloc(sizeof(Espaco));
		aux->prox->t=cap/8;
		aux->prox->tInicial=cap/8;
		aux->prox->arq=NULL;
		aux=aux->prox;
	}
	aux->prox=NULL;
}
void exibe (Espaco *d)
{
	Arquivo *arq;
	while(d!=NULL)
	{
		if(d->t > d->tInicial/100*75)
			printf("[ ]",d->t,d->tInicial);
		else
		if(d->t > d->tInicial/100*25)
			printf("[-]",d->t,d->tInicial);
		else
			printf("[#]",d->t,d->tInicial);
		arq=d->arq;
		while(arq!=NULL)
		{
			printf(" - %s|%d",arq->nome,arq->t);
			arq=arq->prox;
		}
		printf("\n");
		d=d->prox;
	}	
}
void saida(Fila *f)
{
	Espaco *d;
	while(f!=NULL)
	{
		if(f->disco!=NULL)
		{
			d=f->disco;
			while(d!=NULL)
			{
				if(d->t > d->tInicial/100*75)
					printf("[ ]",d->t,d->tInicial);
				else
				if(d->t > d->tInicial/100*25)
					printf("[-]",d->t,d->tInicial);
				else
					printf("[#]",d->t,d->tInicial);
				d=d->prox;
			}
			printf("\n");
		}
		else
			printf("Disco cheio!\n");
		f=f->prox;
	}
	getch();
}
void addfila(Espaco *d,Fila **f)
{
	Fila *aux,*novo;
	novo=(Fila*)malloc(sizeof(Fila));
	novo->disco=d;
	novo->prox=NULL;
	if(*f==NULL)
		*f=novo;
	else
	{
		aux=*f;
		while(aux->prox!=NULL)
		{
			aux=aux->prox;
		}
		aux->prox=novo;
	}
}
int menu()
{
	Fila *f=NULL;
	char op[50], nome[10], tamanho[10],discocheio=false;
	Espaco *disco;
	int operacoes, cap;
	printf("\nDigite o quantidade de operacoes: ");
	scanf("%d",&operacoes);
	fflush(stdin);
	while(operacoes!=0)
	{
		do
		{
			printf("\nDigite o capacidade de disco(somente multiplo de 8KB): ");
			scanf("%s",&tamanho);
			fflush(stdin);
			cap=conversor(tamanho);
			if(cap%8!=0)
				printf("\nValor invalido!\n");
		}while(cap%8!=0);
		criardisco(&disco,cap);
		
		while(operacoes>0)
		{
			printf("\nDigite os comandos de operacoes: ");
			gets(op);
			separar(op,nome,tamanho);
			fflush(stdin);
			if(strcmp(op,"insere")==0)
			{
			
				if(inserir(nome,conversor(tamanho),disco))
					discocheio=true;
			}
			else if (strcmp("remove",op)==0)
			{
				remover(nome,disco);
				
			}
			else
			{
				printf("\nComando errado! ");
				operacoes++;
			}
				
			
			operacoes--;
		}
		if(discocheio)
			disco=NULL;
		
		addfila(disco,&f);
		discocheio=false;
		printf("\nDigite o quantidade de operacoes: ");
		scanf("%d",&operacoes);
		fflush(stdin);
	}
	saida(f);
	
}
int main()
{
	menu();
	
	
}
