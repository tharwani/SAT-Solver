#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
int s_no(int i,int j,int k){
    return (i*81)+(j*9)+(k*1)+1;
}
int rev(int a){
    int y = 0;
    int i = 1;
    while(a/(int)pow(10,i)!=0) i++;
    i = i-1;
    for(int j=i;j>=0;j--){
        y += (a/(int)pow(10,j))*(int)pow(10,i-j);
        a = a%(int)pow(10,j);
    }
    return y;
}
int main(){
    srand(time(0)); // seeding the starting of rand() to 0 
    FILE* fptr = fopen("output.txt","r"); // open the output.txt to read the complete sudoku encoding
    char c1[10];
    char c2[10000]; // stores complete sudoku encoding
    if(fptr==NULL){
        exit(1);
    }
    fgets(c1,1000,fptr);
    if(strcmp(c1,"SAT\n")!=0){
        exit(1);
    }
    fgets(c2,10000,fptr);
    fclose(fptr);
    // sud will stores literals whose values were true in c2
    int sud[81];
    int i;
    for(i=0;i<81;i++){
        sud[i]=0;
    }
    int s = 0;
    i = 0;
    if(c2[i]=='1'){
        sud[s] = 1;
        s++;
    }
    while(c2[i]!='\n'){
        if((c2[i]==' ')&&(c2[i+1]!='-')&&(c2[i+1]!='0')){
            int j;
            for(j=1;c2[i+j]!=' ';j++){
                sud[s] += (c2[i+j]-'0')*(int)pow(10,j-1);
            }
            sud[s] = rev(sud[s]);
            if(c2[i+j-1]=='0'&&c2[i+j-2]=='0') sud[s] = sud[s]*100;
            else if(c2[i+j-1]=='0') sud[s]=sud[s]*10;
            s++;
            i = i+j-1;
        }
        i++;
    }
    // c_buffer will store all encodings of sudoku and negation of c2 in order to get new solutions
    char c_buffer[1000000];
    int j,k;
    int cnt = 0; // stores total number of sudoku encondings
    // pushing sudoku encodings in c_buffer
    for(j=0;j<9;j++){
        for(k=0;k<9;k++){
            for(i=0;i<9;i++){
                char temp[10];
                sprintf(temp,"%d ",s_no(i,j,k));
                strcat(c_buffer,temp);
            }
            char temp[10];
            sprintf(temp,"0\n");
            strcat(c_buffer,temp);
            cnt++;
        }
    }
    //
    for(j=0;j<9;j++){
        for(k=0;k<9;k++){
            for(i=0;i<9;i++){
                for(int l=0;l<i;l++){
                    char temp[30];
                    sprintf(temp,"%d %d %d\n",-(s_no(l,j,k)),-(s_no(i,j,k)),0);
                    strcat(c_buffer,temp);
                    cnt++;
                }
            }
        }
    }
    //
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(k=0;k<9;k++){
                for(int l=0;l<k;l++){
                    char temp[30];
                    sprintf(temp,"%d %d %d\n",-(s_no(i,j,l)),-(s_no(i,j,k)),0);
                    strcat(c_buffer,temp);
                    cnt++;
                }
            }
        }
    }
    //
    for(i=0;i<9;i++){
        for(k=0;k<9;k++){
            for(j=0;j<9;j++){
                for(int l=0;l<j;l++){
                    char temp[30];
                    sprintf(temp,"%d %d %d\n",-(s_no(i,l,k)),-(s_no(i,j,k)),0);
                    strcat(c_buffer,temp);
                    cnt++;
                }
            }
        }
    }
    //
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(int l=0;l<j;l++){
                char temp[30];
                sprintf(temp,"%d %d %d\n",-(s_no(i,l,l)),-(s_no(i,j,j)),0);
                strcat(c_buffer,temp);
                cnt++;
            }
        }
    }
    //
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(int l=0;l<j;l++){
                char temp[30];
                sprintf(temp,"%d %d %d\n",-(s_no(i,l,8-l)),-(s_no(i,j,8-j)),0);
                strcat(c_buffer,temp);
                cnt++;
            }
        }
    }
    //
    for(i=0;i<9;i++){
        for(int m=0;m<3;m++){
            for(int n=0;n<3;n++){
                for(j=3*m;j<3*(m+1);j++){
                    for(k=3*n;k<3*(n+1);k++){
                        for(int p=3*m;p<=j;p++){
                            for(int q=3*n;((q<k)||(p<j))&&(q<3*(n+1));q++){
                                char temp[30];
                                sprintf(temp,"%d %d %d\n",-(s_no(i,p,q)),-(s_no(i,j,k)),0);
                                strcat(c_buffer,temp);
                                cnt++;
                            }
                        }
                    }
                }
            }
        }
    }
    //
    // to store negation of c2 in str2
    char str2[10000];
    i = 0;
    j = 0;
    while(c2[i]!='\n'){
        if(i==0 && c2[i]!='-'){
            str2[j]='-';
            j++;
            str2[j]=c2[i];
        }
        else if(c2[i]=='-'){
            i++;
            continue;
        }
        else if(c2[i]==' ' && c2[i+1]!='0' && c2[i+1]!='\n' && c2[i+1]!='-'){
            str2[j] = ' ';
            str2[j+1] = '-';
            j++; 
        }
        else str2[j] = c2[i];
        i++;
        j++;
    }
    str2[j] = c2[i];
    strncat(c_buffer,str2,j+20); // pusing str2 in c_buffer
    //
    char* cmd;
    int last; // stores last random entry picked out of sudoku after a loop
    i=0;
    while(1){
        int ran = rand()%(81-i); // randomly picking sudoku entries which are to be removed
        last = sud[ran]; // storing the picked entry
        int temp[81-i]; // it is copy of sud, it will help in shrinking the size of sud
        for(int i1=0;i1<81-i;i1++){
            temp[i1]=sud[i1];
        }
        // removing last from sud
        int k1 = 0;
        int k2 = 0;
        while(k1<81-i){
            if(k1==ran) k1++;
            sud[k2] = temp[k1];
            k1++;
            k2++;
        }
        // opening input2.txt to write input to minisat
        fptr = fopen("input2.txt","w");
        fprintf(fptr,"p cnf 729 %d\n",cnt+81-i); // no of clauses will be cnt+1+(80-i) 
        // pushing the clauses from new sud to input2.txt
        for(k1=0;k1<80-i;k1++){
            fprintf(fptr,"%d 0\n",sud[k1]);
        }
        fprintf(fptr,"%s",c_buffer); // pushing c_buffer to input2.txt
        fclose(fptr);
        // invoking minisat to get output in output2.txt
        cmd = "minisat input2.txt output2.txt";
        system(cmd);
        // checking the first line of output2.txt
        fptr = fopen("output2.txt","r");
        char c[10]; // stores first line of output2.txt
        fgets(c,10,fptr);
        fclose(fptr);
        int flag = 0;
        // When we first come across "SAT" output, we have to check whether all positions in that sudoku are giving the same in order for the sudoku to be minimal for unique solution.  
        if(strcmp(c,"UNSAT\n")!=0) {
            flag = 1;
            sud[80-i] = last;
            int temp1[80-i];
            int l = 0;
            while(l<81-i){
                int k3 = 0;
                int k4 = 0;
                while(k3<81-i){
                    if(k3==l) k3++;
                    temp1[k4] = sud[k3];
                    k3++;
                    k4++;
                }
                fptr = fopen("input2.txt","w");
                fprintf(fptr,"p cnf 729 %d\n",cnt+81-i);
                for(k3=0;k3<80-i;k3++){
                    fprintf(fptr,"%d 0\n",temp1[k3]);
                }
                fprintf(fptr,"%s",c_buffer); // pushing c_buffer to input2.txt
                fclose(fptr);
                cmd = "minisat input2.txt output2.txt";
                system(cmd);
                fptr = fopen("output2.txt","r");
                char c3[10];
                fgets(c3,10,fptr);
                fclose(fptr);
                if(strcmp(c3,"UNSAT\n")==0){
                    flag = 0;
                    break;
                }
                l++;
            }
            if(flag==0){
                for(int i1=0;i1<80-i;i1++){
                    sud[i1]=temp1[i1];
                }
            }
        } // if first line is UNSAT then loop continues, else SAT condition is reach, so we break out of loop
        if(flag==1) break;
        if(i>=80) break;
        ++i;
    }
    // to print the minimal sudoku
    //sud[80-i]=last; // restoring the minimal value for which sudoku had unique solution
    int len_sud = 81-i; // length of sud
    int sudoku[9][9];
    // '0' in sudoku denotes empty cell
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            sudoku[i][j] = 0;
        }
    }
    // decoding sud for sudoku
    for(i=0;i<len_sud;i++){
        sud[i] -= 1;
    }
    for(i=0;i<len_sud;i++){
        int m = sud[i]/81;
        sud[i] = sud[i]%81;
        int n = sud[i]/9;
        sud[i] = sud[i]%9;
        int o = sud[i];
        sudoku[n][o] = m+1;
    }
    //
    for(i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(sudoku[i][j]==0) printf(". ");
            else printf("%d ",sudoku[i][j]);
        }
        printf("\n");
    }
    return 0; 
}
