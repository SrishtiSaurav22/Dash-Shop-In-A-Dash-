/*
Data Structures & Algorithms Course Project

Semester: 3
Division: E

Group Details:

Srishti Saurav (529)
Sreevalli Chowdary (528)
Priyam Karn (536)
Vibhuti Raj (519)

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

typedef struct user
{
    int id;
    char name[30];
}user;

typedef struct product_details
{
    int id;
    char brand[30];
    char name[30];
    int price;
    char category[30];
}pd;

typedef struct priority_queue
{
    int node;
    int wt; //consideration factor which will consider all traffic weather and edge length
    int weight_covered; //this will only consider the edge length without any other factors
}priority_queue;

typedef struct nodes
{
    int traffic;
    int edge_weight;
    int weather;
}nodes;

int str_to_num(char str[])
{
    //this function returns the integer equivalent of the given string

    int num=0;

    for(int i=0;str[i]!='\0';i++)
        num=(num*10)+(str[i]-'0');

    return num;
}

int isEmpty(priority_queue *pq,int n)
{
    //returns if priority queue is empty or not

    for(int i=0;i<n;i++)
    {
        if(pq[i].wt!=-1 || pq[i].node!=-1)
        return 0;
    }

    return 1;
}

priority_queue front(priority_queue *pq,int n)
{
    //returns the node with least edge weight

    int w=INT_MAX;

    priority_queue temp;

    for(int i=0;i<n;i++)
    {
        if(pq[i].wt<w && pq[i].wt!=-1)
        {
            w=pq[i].wt;
            temp=pq[i];
        }
    }

    return temp;
}

void dequeue(priority_queue *pq,int n,priority_queue temp)
{
    //deletes the minimum most element from the priority queue
    //in other words we will mark that particular node and weight with useless value like -1

    for(int i=0;i<n;i++)
    {
        if(temp.node==pq[i].node && temp.wt==pq[i].wt)
        {
            pq[i].wt=-1;
            pq[i].node=-1;
        }
    }
}

void dijkstra(int dest)
{
    //function for dijkstra's algorithm
    //this function finds shortest path to deliver product after considering all 3 factors i.e traffic,weather and edge weight

    priority_queue pq[200];

    for(int i=0;i<200;i++)
    {
        pq[i].node=-1;
        pq[i].wt=-1;
    }

    int l=0;
    int src=0;//assuming the shop is at 0th node
    char num[20];

    FILE *ptr=fopen("graph_file.txt","r");
    fscanf(ptr,"%s",num);

    int n=str_to_num(num);

    //reading the map from the file

    nodes **arr=(nodes **)malloc(sizeof(nodes *)*n);

    for(int i=0;i<n;i++)
    {
        arr[i]=(nodes *)malloc(sizeof(nodes)*n);

        for(int j=0;j<n;j++)
        {
            char edge_weight[20];
            char traffic[20];
            char weather[20];

            fscanf(ptr,"%s",edge_weight);
            fscanf(ptr,"%s",traffic);
            fscanf(ptr,"%s",weather);

            arr[i][j].edge_weight=str_to_num(edge_weight);
            arr[i][j].traffic=str_to_num(traffic);
            arr[i][j].weather=str_to_num(weather);

            //printf("( %d %d %d ) ",arr[i][j].edge_weight,arr[i][j].traffic,arr[i][j].weather);
        }

        //printf("\n");
    }

    //the distance array will also be modifies as it takes all factors into consideration
    //the first row will store all the weights including consideration factor
    //the second row will store only the distance

    int distance[2][n];

    for(int i=0;i<n;i++)
    {
        distance[0][i]=772681;
        distance[1][i]=0;
    }

    distance[0][src]=0;

    pq[l].node=src;
    pq[l].wt=0;
    pq[l].weight_covered=0;

    while(!isEmpty(pq,l+1))
    {
        priority_queue temp=front(pq,l+1);
        dequeue(pq,l+1,temp);

        int node=temp.node;
        int curr_dist=temp.wt;
        int weight_covered_till_now=temp.weight_covered;

        for(int i=0;i<n;i++)
        {
            //considering all factors

            int next_dist=curr_dist+(arr[node][i].traffic+arr[node][i].weather+arr[node][i].edge_weight);

            if(arr[node][i].edge_weight!=0 && next_dist<distance[0][i])
            {
                distance[0][i]=next_dist;
                l=l+1;

                pq[l].node=i;
                pq[l].wt=next_dist;
                pq[l].weight_covered=weight_covered_till_now+arr[node][i].edge_weight;

                distance[1][i]=pq[l].weight_covered;
            }
        }
    }

    printf("\nShortest distance between shop and your location is %d KM.",distance[0][dest]);
    //assuming that to travel consideration factor 1 it takes 1.5 minute of time
    printf("\nTotal time for product to be delivered is %f minutes\n",distance[0][dest]*1.5);
}


void select_product()
{
    int i;

    FILE *ptr=fopen("products.txt","r");

    int num=62; //number of products

    pd products[num];

    int l=0;

    while(!feof(ptr))
    {
        char id[10];

        fscanf(ptr,"%s",id);
        products[l].id=str_to_num(id);

        fscanf(ptr,"%s",products[l].brand);
        fscanf(ptr,"%s",products[l].name);

        char price[10];

        fscanf(ptr,"%s",price);
        products[l].price=str_to_num(price);

        fscanf(ptr,"%s",products[l].category);

        l=l+1;
    }

    int lower=1;
    int upper=10;

    int rand_freq=(rand()%(upper-lower+1))+lower; //user can shop for max 10 items

    int product_code;

    FILE *bill;

    for(int i=0;i<rand_freq;i++)
    {
        // id will be between 1 and the number of items
        int lower=1;
        int upper=num;

        int rand_prod=(rand()%(upper-lower+1))+lower;

        fflush(stdin);

        // the items being chosen are not printing, so this line has been commented
        //printf("\n\nItems being added to the cart:\n");

        for(int i=0;i<num;i++)
        {
            //exact matching
            if(rand_prod==(i+1))
            {
                // the items being chosen are not printing, so this line has been commented
                //printf("%d %s %s Rs. %d\n",i+1,products[i].brand,products[i].name,products[i].price);

                bill=fopen("final_bill_temp.txt","a");

                // max quantity is 10
                int lower=1;
                int upper=10;

                int rand_qty=(rand()%(upper-lower+1))+lower;
                product_code=i+1;

                long long int cost=products[product_code].price*rand_qty;

                fprintf(bill,"%d %s %s %d %lld .\n",product_code,products[product_code].brand,products[product_code].name,rand_qty,cost);
                fclose(bill);
            }
        }
    }

    printf("\n");
        for(i=0;i<210;i++)
            printf("_");

    printf("\nITEMS BOUGHT:\n");

            long long int total=0;

            bill=fopen("final_bill_temp.txt","r");
            printf("\nItem_Code\tBrand\t\tProduct\t\t\tQuantity\t\tPrice\n");

            pd product_recommendations[4];

            int flag_for_recc=0;//flag to denote recommendation
            int index_rec=0;

            while(!feof(bill))
            {
                char prod_code[10];
                char prod_brand[30];
                char prod_name[30];
                char prod_qty[30];
                char prod_price[30];
                char garbage_read[10];

                fscanf(bill,"%s",prod_code);
                fscanf(bill,"%s",prod_brand);
                fscanf(bill,"%s",prod_name);
                fscanf(bill,"%s",prod_qty);
                fscanf(bill,"%s",prod_price);
                fscanf(bill,"%s",garbage_read);

                printf("%s\t%s\t\t%s\t\t\t%s\t\t%s\n",prod_code,prod_brand,prod_name,prod_qty,prod_price);

                if(flag_for_recc==0)
                {
                    for(int i=0;i<num;i++)
                    {
                        if(str_to_num(prod_code)!=i)
                        {
                            //recommending some other item not the same
                            if(index_rec<2)
                            {
                                //we will recommend 2 items of same brand
                                if(strcasecmp(prod_brand,products[i].brand)==0)
                                {
                                    product_recommendations[index_rec]=products[i];
                                    index_rec+=1;
                                }
                            }

                            else if(index_rec<4)
                            {
                                //2 items of same category but of different brand

                                char p_category[30];

                                strcpy(p_category,products[str_to_num(prod_code)].category);

                                if(strcasecmp(p_category,products[i].category)==0 && strcmp(prod_brand,products[i].brand)!=0)
                                {
                                    product_recommendations[index_rec]=products[i];
                                    index_rec+=1;
                                }
                            }
                        }
                    }

                    flag_for_recc=1;
                }

                total=total+str_to_num(prod_price);
            }

            fclose(bill);

            printf("\n\n");

            printf("TOTAL BILL: Rs. %lld\n",total);

            for(i=0;i<210;i++)
                printf("_");

            printf("\nYou may consider buying the following products:");
            printf("\n\nBrand\t\tProduct\t\tCategory\t\tPrice\n");

            for(int i=0;i<index_rec;i++)
                printf("%s\t\t%s\t\t%s\t\t\tRs. %d\n",product_recommendations[i].brand,product_recommendations[i].name,product_recommendations[i].category,product_recommendations[i].price);

            printf("\n");
            for(i=0;i<210;i++)
                printf("_");

            lower=0;
            upper=8;

            int loc=(rand()%(upper-lower+1))+lower;

            printf("\nLocation code: %d",loc);

            printf("\nYour products are out for delivery!\n");

            dijkstra(loc);
}

int search(char* pat, char* txt)
{
    int M=strlen(pat);
    int N=strlen(txt);

    int i;

    for(i=0;i<=N-M;i++)
    {
        int j;

        for(j=0;j<M;j++)
            if(txt[i+j]!=pat[j])
                break;

        if(j==M)
            return 1;
    }

    return 0;
}

typedef struct keyword
{
    int num;
    char word[20];
} kw;

void display(char *str)
{
    int i;

    for(i=0;i<strlen(str);i++)
    {
        if(str[i]=='_')
            printf(" ");

        else
            printf("%c",str[i]);
    }
}

void query_solution()
{
    /*
    Store the sentences in the file where the words are separated by underscore rather than space,
    however, while printing replace every underscore with a space.
    */

    kw list[5];

    list[0].num=0;
    strcpy(list[0].word,"return");

    list[1].num=1;
    strcpy(list[1].word,"replacement");

    list[2].num=2;
    strcpy(list[2].word,"cashback");

    list[3].num=3;
    strcpy(list[3].word,"exchange");

    list[4].num=4;
    strcpy(list[4].word,"cancel");

    char qry[500];
    char sol[500];
    char to_search[50];

    int lower=0;
    int upper=4;

    int rand_word=(rand()%(upper-lower+1))+lower;

    for(int i=0;i<5;i++)
        if(i==rand_word)
            strcpy(to_search,list[i].word);

    FILE *s,*q;

    q=fopen("query.txt","r");

    while(!feof(q))
    {
        fscanf(q,"%s",qry);

        if(search(to_search,qry)==1)
        {
                s=fopen("solutions.txt","r");

                while(!feof(s))
                {
                    fscanf(s,"%s",sol);

                    if(search(to_search,sol)==1)
                    {
                        fflush(stdin);
                        display(qry);
                        printf("\n\n");
                        fflush(stdin);
                        display(sol);
                        break;
                    }
                }

                fclose(s);
        }
    }

    fclose(q);
}

int main()
{
    int i;

    int no_of_users=0;

    int id;
    char name[30];

    FILE *ufile;

    ufile=fopen("users.txt","r");

    while(!feof(ufile))
    {
        fscanf(ufile,"%d",&id);
        fscanf(ufile,"%s",&name);
        ++no_of_users;
    }

    fclose(ufile);

    int lower=0;
    int upper=no_of_users;

    int rand_user=(rand()%(upper-lower+1))+lower;

    ufile=fopen("users.txt","r");

    while(!feof(ufile))
    {
        fscanf(ufile,"%d",&id);
        fscanf(ufile,"%s",name);

        if(id==rand_user)
            break;
    }

    fclose(ufile);

    printf("\nWelcome to Dash!\n\nShop in a dash!");

    printf("\n");
            for(i=0;i<210;i++)
                printf("_");

    printf("\n\nSelected user:\nID: %d\nName: %s",id,name);

    //for delaying the display of selected user

    int c,d;

    for(c=1;c<=32767;c++)
       for(d=1;d<=32767;d++)
       {}

    select_product();

    printf("\n");
            for(i=0;i<210;i++)
                printf("_");

    printf("\nFAQs:\n\n");
    query_solution();

    printf("\n");
            for(i=0;i<210;i++)
                printf("_");

    printf("\nThank you for shopping with us!");

    printf("\n");
            for(i=0;i<210;i++)
                printf("_");

    printf("\n\n");
    return 0;
}




