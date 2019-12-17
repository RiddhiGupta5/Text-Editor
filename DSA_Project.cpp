#include<iostream>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include<cstdlib>
#include<fstream>
#include<conio.h>

using namespace std;

const int SIZE=27; //Size of Array in trie data structure

struct NW //NW=NextWords
{
    char nextWord[20];
    NW *Link;
};

struct MLL //MLL=MainLinkedList
{
    char word[20];
    NW *head;
    int Count; //Keeps count of next words
    int isFirstWord; //will help in determining whether this was the first word in the sentences entered by the user
    int index;
    MLL *next;
};

struct TrieNode
{
    char word[20];
    int isWord;
    TrieNode *letters[SIZE];
};


MLL *MLLhead=NULL;
TrieNode *root=NULL;
int countFirstWord=0,FirstWord[100]={},nNode=0; //FirstWord stores all index to all first words,
const int nSuggestions=5;
char suggestionsList[nSuggestions][20];

void frontPage();
void display(); //*****FOR DEMO
void deleteMLL();
void deleteTrie(TrieNode* node);
MLL *ExistingNode(char[]);
void createLinkedList(char[]);
void createNewNode(char[],char[]);
void EnterText(char[]);
void markovTextGenerator(int);
void createDict();
void insertTrieNode(char[]);
TrieNode *getNewTrieNode();
int searchWord(char[]);
int hashFunction(char);
int checkWord(char ww[][20]);
int spellingSuggestions(char[]);
int presentInArray(char word[],char Array[][20],int a);
int substitutions(char word[],char temp[][20],int nWords);
int distanceOne(char word[],char temp[][20]);
int insertions(char word[],char temp[][20],int nWords);
int deletions(char word[],char temp[][20],int nWords);

int main()
{
    char Text[2000],wrongWords[1000][20];
    int nSentence,choice,nWrongWords,call=0,i,j,numSg=0;
    createDict();
    frontPage();
    EnterText(Text);
    createLinkedList(Text);
    //cout<<"\n\n\nFinal state:\n";
    //display();
    cout<<"\n..........Thank you for entering text............\n";
    do
    {
        cout<<"\n";
        cout<<"\nPress 1. To find spelling mistakes";
        cout<<"\nPress 2. To get some suggestions for the misspelled words";
        cout<<"\nPress 3. To generate some random text";
        cout<<"\nPress 4. To Exit";
        cout<<"\nEnter your choice: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            nWrongWords=checkWord(wrongWords);
            if(nWrongWords!=0)
            {
                cout<<"\nThe misspelled words in the text are:\n";
                for(i=0;i<nWrongWords;i++)
                {
                    cout<<wrongWords[i]<<"\n";
                }
            }
            else
            {
                cout<<"\nThere are no misspelled words\n";
            }
            call=1;
            break;
        case 2:

            if(call!=1)
            {
                cout<<"\nCalling check words";
                nWrongWords=checkWord(wrongWords);
            }
            if(nWrongWords==0)
            {
                cout<<"\nThere are no misspelled words\n";
            }
            else
            {
                cout<<"\nThe following are the misspelled words entered by you and their corresponding suggestions:\n";
                for(i=0; i<nWrongWords ;i++)
                {
                    numSg=0;
                    numSg=spellingSuggestions(wrongWords[i]);
                    cout<<"\n"<<wrongWords[i]<<": ";
                    if(numSg==0)
                    {
                        cout<<"Cannot find suggestions";
                    }
                    for(j=0;j<nSuggestions;j++)
                    {
                        cout<<suggestionsList[j]<<" ";
                    }

                }
            }
            break;
        case 3:
            cout<<"\nEnter number of Sentences you want to generate: ";
            cin>>nSentence;
            markovTextGenerator(nSentence);
            cout<<"\n";
            break;
        case 4:
            exit(0);
        default:
            cout<<"\nInvalid choice";
            break;
        }

    }while(1);
    deleteMLL();
    deleteTrie(root);
}

void frontPage() //Makes the first page---so that console looks user friendly
{
    char c;
    for(int i=0;i<86;i++)
    {
        cout<<"="<<" ";
    }
    for(int i=0;i<16;i++)
    {
        cout<<"||";
        for(int i=0;i<84;i++)
        {
            cout<<"  ";
        }
        cout<<"||";
        //cout<<endl;
    }
    cout<<"||                                                                              DSA PROJECT                                                                               ||";
    cout<<"||                                                                              TEXT EDITOR                                                                               ||";
    cout<<"||                                                                        #MARKOV TEXT GENERATION                                                                         ||";
    cout<<"||                                                                             #SPELL CHECK                                                                               ||";
    cout<<"||                                                                         #SPELLING SUGGESTIONS                                                                          ||";

    for(int i=0;i<16;i++)
    {
        cout<<"||";
        for(int i=0;i<84;i++)
        {
            cout<<"  ";
        }
        cout<<"||";
        //cout<<endl;
    }
    for(int i=0;i<86;i++)
    {
        cout<<"="<<" ";
    }
    cout<<"\n\n\n";
    cout<<"ABOUT THE TEXT EDITOR:";
    cout<<"\n\n\n";
    cout<<"This is developing model of the Text Editor.";
    cout<<"\n\n";
    cout<<"** This is a Text Editor that will help you to generate some random Markov Text, check for spelling \n   mistakes and ";
    cout<<"will suggest you in order to improve your spelling mistakes. \n** This is a special Text Editor that has some special ";
    cout<<"functionality that is random text generation integrated along with some basic functionalities of a Text Editor like \n";
    cout<<"   Selling check and Spelling suggestions. \n** Markov text generation is the process of predicting some possible future states ";
    cout<<"depending upon the present states. \n** You can use the random text generation functionality of this Text Editor to create some ";
    cout<<"poetries or raps etc. \n** This can especially help in making song lyrics as the nature of the sentence is preserved here.";
    cout<<"\n\nEnter a character to continue...........";
    getch();
    cout<<"\n\n\n";
}


void EnterText(char Text[]) //will store text entered by the user
{
    cout<<"Enter your text (word limit 2000 characters)\nEnter enter character to stop entering text\n";
    gets(Text);
}

void display()  //for displaying the main linked list
{               //**** FOR DEMO
    MLL *temp=MLLhead;
    NW *tempNext;
    cout<<"\nDisplaying words stored in linked list\n";
    while(temp!=NULL)
    {
        cout<<"\nWord stored at the Node:"<<temp->word<<"\n";
        cout<<"Next word count:"<<temp->Count<<"\n";
        cout<<"Is First word:"<<temp->isFirstWord<<"\n";
        cout<<"Index:"<<temp->index<<"\n";
        cout<<"Corresponding Next words:\n";
        tempNext=temp->head;
        while(tempNext!=NULL)
        {
            cout<<tempNext->nextWord<<"\n";
            tempNext=tempNext->Link;
        }
        temp=temp->next;
    }
    cout<<"Total number of words:"<<nNode<<"\n";
    cout<<"Total number of first words:"<<countFirstWord<<"\n";
    cout<<"Index of first words:\n";
    for(int i=0;i<countFirstWord;i++)
    {
        cout<<FirstWord[i]<<" ";
    }
    cout<<"\n";
}



MLL *ExistingNode(char w[]) //checks whether a word exists or not in the main linked list
{
    MLL *temp=MLLhead;
    while(temp!=NULL)
    {
        if(strcmp(temp->word,w)==0)
        {
            return temp;
        }
        temp=temp->next;
    }
    return temp;
}




void createNewNode(char w[],char nw[],int fw) //Creates a new node for a word if it does not exist otherwise it modifies its next word linked list.
{                                             //This also updates the count of first word and first words so that we can keep track of starting words

    NW *tempNext=new NW;
    MLL *address;
    strcpy(tempNext->nextWord,nw);
    tempNext->Link=NULL;
    address=ExistingNode(w);
    if(address==NULL)
    {
        nNode++;
        MLL *temp=new MLL;
        temp->index=nNode;
        strcpy(temp->word,w);
        temp->next=NULL;
        temp->Count=1;
        temp->isFirstWord=fw;
        temp->head=tempNext;
        if(fw==1)
        {
            FirstWord[countFirstWord]=nNode;
            countFirstWord++;
        }
        if(MLLhead==NULL)
        {
            MLLhead=temp;
        }
        else
        {
            MLL *temp2=MLLhead;
            while(temp2->next!=NULL)
            {
                temp2=temp2->next;
            }
            temp2->next=temp;
        }
    }
    else
    {
        if(fw==1)
        {
            address->isFirstWord=fw;
            FirstWord[countFirstWord]=address->index;
            countFirstWord++;
        }

        address->Count=address->Count+1;
        NW *temp2=address->head;
        while(temp2->Link!=NULL)
        {
            temp2=temp2->Link;
        }
        temp2->Link=tempNext;
    }
}




void createLinkedList(char Text[]) //will create main linked list
{
    int w=0,f=1;
    char word[20],prevWord[20];
    strcpy(prevWord,"\0");
    for(int i=0;i<strlen(Text);i++)
    {
        if(Text[i]!=' '&&Text[i]!='\0'&&Text[i]!='.'&&Text[i]!=',')
        {
            word[w]=tolower(Text[i]);
            w++;
            if(Text[i+1]==' '||Text[i+1]=='.'||Text[i+1]=='\0'||Text[i+1]==',')
            {

                word[w]='\0';
                if(strcmp(prevWord,"\0")==0 && (Text[i+1]=='\0'||Text[i+1]=='.')) //will create a new node in main linked list for single word entered in text
                {
                    createNewNode(word,".",f);
                }
                else if(strcmp(prevWord,"\0")==0) //helps in setting the prevword for the first word and text contains more than one word
                {
                    strcpy(prevWord,word);
                    f=1;
                }
                else //will work for inserting node in main linked list for all other cases than single word entered in text
                {
                    createNewNode(prevWord,word,f);
                    f=0;
                    strcpy(prevWord,word);
                    if(Text[i+1]=='.'||Text[i+1]=='\0'||Text[i+1]==',') //will help in setting the prevword accordingly if the word is the last word in the sentence
                    {                                                   //and creating a new node
                        createNewNode(prevWord,".",f);
                        strcpy(prevWord,"\0");
                    }
                }


            }
        }
        else
        {
            strcpy(word," ");
            w=0;

        }

    }
}



void markovTextGenerator(int nS) //Generates random text
{
    srand(time(0));
    MLL *temp;
    char nWord[20];
    int j,i;
    int first,index,nIndex;
    for(i=1;i<=nS;i++)
    {
        temp=MLLhead;
        first=rand()%countFirstWord;
        index=FirstWord[first];
       for(j=1;j<index;j++)
        {
            temp=temp->next;
        }
        for(int l=0;l<strlen(temp->word);l++)
        {
            if(l==0)
                cout<<(char)(toupper((temp->word)[0]));
            else
                cout<<(temp->word)[l];
        }
        cout<<" ";
        do
        {
            nIndex=rand()%(temp->Count);
            NW *Word=temp->head;
            for(j=0;j<nIndex;j++)
            {
                Word=Word->Link;
            }
            strcpy(nWord,Word->nextWord);
            if(strcmp(nWord,".")==0)
                cout<<"\b";
            cout<<nWord<<" ";
            temp=ExistingNode(nWord);
        }while(strcmp(nWord,".")!=0);

    }
}



TrieNode *getNewTrieNode() //allocate new node of TrieNode type in heap
{
    TrieNode *temp=new TrieNode;
    temp->isWord=0;
    strcpy(temp->word," ");
    for(int i=0;i<SIZE;i++)
    {
        temp->letters[i]=NULL;
    }
    return temp;
}



void insertTrieNode(char w[]) //will insert a new word in trie if it does not exist
{
    int index;
    if(root==NULL)
    {
        root=getNewTrieNode();
    }

    TrieNode *temp=root;
    for(int i=0;i<strlen(w);i++)
    {
         index=hashFunction(w[i]);
        if(temp->letters[index]==NULL)
        {
            temp->letters[index]=getNewTrieNode();
        }
        temp=temp->letters[index];
    }
    strcpy(temp->word,w);
    temp->isWord=1;
}



void createDict() //Will form the trie data structure that stores all the words in a dictionary
{
    ifstream dict;
    char word[20];
    dict.open("FinalDict.txt",ios::in);
    while(!dict.eof())
    {
        dict>>word;
        //cout<<"\n"<<word;
        insertTrieNode(word);
    }
}



int hashFunction(char c) //for finding suitable index for a character in an array of size 27
{
    int index;
    if(c=='\'')
    {
        index=26;
    }
    else
    {
        index=(tolower(c)-97);
    }
    return index;
}



int searchWord(char w[]) //searches word in the trie and returns 1 if found otherwise returns 0
{
    TrieNode *temp=root;
    int index;
    for(int i=0;i<strlen(w);i++)
    {
        index=hashFunction(w[i]);
        if(temp->letters[index]==NULL)
            return 0;
        else
            temp=temp->letters[index];
    }
    if(temp->isWord==1 && temp!=NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}




int checkWord(char ww[][20]) //checks whether the word is a wrong word or a correct word
{
    MLL *temp=MLLhead;
    int f,i=0;
    char w[20];
    while(temp!=NULL)
    {
        strcpy(w,temp->word);
        f=searchWord(w);
        if(f!=1)
        {
           strcpy(ww[i],w);
           i++;
        }
        temp=temp->next;

    }
    return i;
}



int spellingSuggestions(char word[]) //Forms suggestions for the word
{
    char Queue[10000][20],curr[20],temp[1000][20],visited[1000][20];
    int qF=-1,qR=-1,v=0,s=0,nWords,n=0;

    qF++;
    qR++;
    strcpy(Queue[qF],word);
    strcpy(visited[v],word);
    v++;
    while(qF!=-1 && s<nSuggestions)
    {
        if(qR==qF)
        {
            strcpy(curr,Queue[qF]);
            qR=-1;
            qF=-1;
        }
        else
        {
            strcpy(curr,Queue[qF++]);
        }

        nWords=distanceOne(curr,temp);
        for(int i=0;i<nWords;i++)
        {
            if(!presentInArray(temp[i],visited,v))
            {
                strcpy(visited[v++],temp[i]);
                if(qF==-1)
                {
                    qF++;
                }
                strcpy(Queue[++qR],temp[i]);
                if(searchWord(temp[i]))
                {
                    strcpy(suggestionsList[s++],temp[i]);
                    n++;
                }

            }
        }
    }
    return n;

}



int distanceOne(char word[],char temp[][20]) //forms the list of all the words which are one character away from the root word
{
    int nWords=0;
    nWords=insertions(word,temp,nWords);
    nWords=substitutions(word,temp,nWords);
    nWords=deletions(word,temp,nWords);
    return nWords;
}



int insertions(char word[],char temp[][20],int nWords) //inserts a char
{
    char newWord[20];
    int j,t;
    for(int i=0;i<strlen(word);i++)
    {
        for(char c='a';c<'z';c++)
        {
            for(j=0,t=0;t<strlen(word);j++,t++)
            {
                if(t==i)
                {
                    newWord[j]=c;
                    newWord[++j]=word[t];
                }
                else
                {
                    newWord[j]=word[t];
                }
            }
            newWord[j]='\0';
            if(!presentInArray(newWord,temp,nWords) && searchWord(newWord) && strcmp(word,newWord)!=0)
            {
                strcpy(temp[nWords++],newWord);
            }
            strcpy(newWord,"");
        }
    }
    return nWords;
}




int deletions(char word[],char temp[][20],int nWords) //deletes a char
{
    char newWord[20];
    int j,t;
    for(int i=0;i<strlen(word);i++)
    {
        for(j=0,t=0;t<strlen(word);j++,t++)
        {
            if(t!=i)
            {
                newWord[j]=word[t];
            }
            else
            {
                t++;
            }
        }
        newWord[j]='\0';
        if(!presentInArray(newWord,temp,nWords) && searchWord(newWord) && strcmp(word,newWord)!=0)
        {
            strcpy(temp[nWords++],newWord);
        }
    }
    return nWords;

}

int substitutions(char word[],char temp[][20],int nWords) //substitutes a char
{
    char newWord[20];
    int j;
    for(int i=0;i<strlen(word);i++)
    {
        for(char c='a';c<'z';c++)
        {
            for(j=0;j<strlen(word);j++)
            {
                if(j==i)
                {
                    newWord[j]=c;
                }
                else
                {
                    newWord[j]=word[j];
                }
            }
            newWord[j]='\0';
            if(!presentInArray(newWord,temp,nWords) && searchWord(newWord) && strcmp(word,newWord)!=0)
            {
                strcpy(temp[nWords],newWord);
            }
        }
    }
}



int presentInArray(char word[],char Array[][20],int a) //checks if the given word is present in a particular array
{
    for(int i=0;i<a;i++)
    {
        if(strcmp(word,Array[i])==0)
        {
            return 1;
        }
    }
    return 0;
}

void deleteMLL()  //for deleting allocated memory for main linked list
{
    MLL *temp=MLLhead,*tM;
    NW *tempNext,*tN;

    while(temp!=NULL)
    {
        tempNext=temp->head;
        while(tempNext!=NULL)
        {
            tN=tempNext;
            tempNext=tempNext->Link;
            delete tN;
        }
        tM=temp;
        temp=temp->next;
        delete tM;
    }
}

void deleteTrie(TrieNode* node)  //for deleting allocated memory for trie data structure
{
    if (node != NULL)
    {
        TrieNode *child[SIZE],*nextNode;
        int n=0;
        for(int i=0;i<SIZE;i++)
        {
            if(node->letters[i]!=NULL)
            {
                child[n]=node->letters[i];
                n++;
            }
        }
        if (n == 0)
        {
            // delete the leaf node
            delete(node);
            return;
        }

        for (int k=0;k<n;k++)
        {
            nextNode=child[k];
            deleteTrie(nextNode);
        }
    }
}
