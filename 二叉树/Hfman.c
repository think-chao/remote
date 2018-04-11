/*
功能：：哈夫曼编码的实现： 
首先根据n个带权值的节点构建n个二叉树，每一个节点作为一个二叉树，其权值保存在节点中，左右子树都是空。
	在这n个二叉树中找到权值最小的两棵树，用这两课树作为左右子树构造新的二叉树，将原来两个数的权值相加作为新的二叉树根节点的权值
	将上步找到的权值最小的两颗二叉树排除在下一次查找范围，重复2,3直到只有一颗树为止，这样就可以得到一颗哈夫曼树
*/ 


//思路：
// 在内存中分配一片连续的区域，用来保存哈夫曼二叉树，可以将这部分内存区域作为一个一维数组
// 然后通过数组的下表序号访问不同的二叉树节点
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct
{
    int weight; //权值 
    int parent; //父结点序号 
    int left; //左子树序号
    int right; //右子树序号 
}HuffmanTree;
typedef char *HuffmanCode;  //Huffman编码 字符型指针 
void SelectNode(HuffmanTree *ht,int n,int *bt1,int *bt2)
//从1~x个结点选择parent结点为0,权重最小的两个结点 
{
     int i;
     HuffmanTree *ht1,*ht2,*t;
     ht1=ht2=NULL; //初始化两个结点为空 
     for(i=1;i<=n;++i) //循环处理1~n个结点（包括叶结点和非叶结点） 
     {
         if(!ht[i].parent) //父结点为空(结点的parent=0) 
         {	
             if(ht1==NULL) //结点指针1为空 
             {
                 ht1=ht+i; //指向第i个结点 
                 continue; //继续循环 
             }
             if(ht2==NULL) //结点指针2为空 
             {
                 ht2=ht+i; //指向第i个结点 
                 if(ht1->weight>ht2->weight) //比较两个结点的权重，使ht1指向的结点权重小 
                 {
                     t=ht2;
                     ht2=ht1;
                     ht1=t;
                 }
                 continue; //继续循环 
             }
             if(ht1 && ht2) //若ht1、ht2两个指针都有效 
             {
                 if(ht[i].weight<=ht1->weight) //第i个结点权重小于ht1指向的结点 
                 {
                     ht2=ht1; //ht2保存ht1，因为这时ht1指向的结点成为第2小的 
                     ht1=ht+i; //ht1指向第i个结点 
                 }else if(ht[i].weight<ht2->weight){ //若第i个结点权重小于ht2指向的结点 
                     ht2=ht+i; //ht2指向第i个结点 
                 }
             }
         }
     }
     if(ht1>ht2){ //增加比较，使二叉树左侧为叶结点 
         *bt2=ht1-ht;
         *bt1=ht2-ht;
     }else{
         *bt1=ht1-ht;
         *bt2=ht2-ht;
     }
}


//ht是一个指向哈夫曼树的指针，n代表创建的哈夫曼树的叶节点个数，w是个指针，指向了一个存放所有节点权重的数组 
//所以叶节点书目为n，总结点个数为2n-1
void CreateTree(HuffmanTree *ht,int n,int *w)
{
    int i,m=2*n-1;//总的节点数
    int bt1,bt2; //二叉树结点序与 
    if(n<=1) return ; //只有一个结点，无法创建 
    for(i=1;i<=n;++i) //初始化叶结点 
    {
        ht[i].weight=w[i-1];
        ht[i].parent=0;
        ht[i].left=0;
        ht[i].right=0;
    }
    for(;i<=m;++i)//初始化后续结点 
    {
        ht[i].weight=0;
        ht[i].parent=0;
        ht[i].left=0;
        ht[i].right=0;
    }
    
    	//现在ht[i]表示每一个节点了 
	
	
	//此时需要计算非叶节点的权重了，因为选取两个权重最小的节点之后
	//需要根据这两个节点的权重之和组成一个二叉树，而这个二叉树的根节点也就是其中一个非叶节点 
    for(i=n+1;i<=m;++i) //逐个计算非叶结点，创建Huffman树 
    {
    	
    	
    	//首先找到两个权重最小的节点，因为最开始大家都是只有一个根节点的二叉树 
		// 找两个权值最小的节点必须从有有权值的n个节点中选择 
		//  
		//希望通过这个函数把找到的最小值的两个节点的序号的值赋值给bt1和bt2
		 
        SelectNode(ht,i-1,&bt1,&bt2); //从1~i-1个结点选择parent结点为0,权重最小的两个结点 
        ht[bt1].parent=i;
        ht[bt2].parent=i;
        ht[i].left=bt1;
        ht[i].right=bt2;
        ht[i].weight=ht[bt1].weight+ht[bt2].weight;
    }
    
    //分派完叶节点之后,每次通过查找到两个最小的权值的叶节点来构成这两个叶节点的根节点
	//根节点的序号从n+1到m 
}

//思路:
//希望从根节点寻找到指定叶节点的路径,从而组成哈夫曼编码
//所以可以从叶节点出发,不断的寻找其父节点,得到其父节点 的编码,然后存放在一个数组里
//最后将的到的数组逆序输出就是该叶节点的哈夫曼编码 
void HuffmanCoding(HuffmanTree *ht,int n,HuffmanCode *hc)//,char *letters)
{
	//cd是一个字符指针,初始给cd一段连续内存的首地址 
     char *cd;
     int start,i;
     int current,parent;    
     cd=(char*)malloc(sizeof(char)*n);//用来临时存放一个字符的编码结果  
     cd[n-1]='\0'; //设置字符串结束标志 
     for(i=1;i<=n;i++)
     {
         start=n-1;
         current=i;
         parent=ht[current].parent;//获取当前结点的父结点 
         while(parent) //父结点不为空 
         {
             if(current==ht[parent].left)//若该结点是父结点的左子树  
             
             //逆序存储每个节点的编码就可以直接得到正确的编码 
               cd[--start]='0'; //编码为0 
             else //若结点是父结点的右子树 
               cd[--start]='1'; //编码为1 
             current=parent; //设置当前结点指向父结点 
             parent=ht[parent].parent; //获取当前结点的父结点序号    
         }
         
         //为什么分配给保存编码的内存是n-start??
		 //start=n-1,假设只循环一次,start=n-1-1
		 //n-start=2 
         hc[i-1]=(char*)malloc(sizeof(char)*(n-start));//分配保存编码的内存 为什么
		 
		  
         strcpy(hc[i-1],&cd[start]); //复制生成的编码           
     }
     free(cd); //释放编码占用的内存 
}

void Encode(HuffmanCode *hc,char *alphabet,char *str,char *code)
//将一个字符串转换为Huffman编码
//hc为Huffman编码表 ,alphabet为对应的字母表,str为需要转换的字符串,code返回转换的结果 
{
     
     int len=0,i=0,j;
     code[0]='\0';
     while(str[i])
     {
         j=0;
         
         //将字母表中的字母与要测的字符串的第一个要编码的字母进行比对
		 // 因为有排好序的字母表存在,所以说要在字母表中要查几次就说明
		 // 需要编码的字母相对于首字母的偏移是多少,从而确定该需要编码的字母
		 
		 // 而且每个字母占有的编码位数是动态的,所以每次需要获取已经编码号的字母的位数
		 // 作为下一个编码数字的偏移 
         while(alphabet[j]!=str[i])
             j++;         
         strcpy(code+len,hc[j]); //将对应字母的Huffman编码复制到code指定位置 
         len=len+strlen(hc[j]); //累加字符串长度 
         i++;
     }
     code[len]='\0';
}


void Decode(HuffmanTree *ht,int m,char *code,char *alphabet,char *decode)
//将一个Huffman编码组成的字符串转换为明文字符串 
//ht为Huffman二叉树,m为字符数量,alphabet为对应的字母表,str为需要转换的字符串,decode返回转换的结果 
{
     int position=0,i,j=0;
     m=2*m-1;
     while(code[position]) //字符串未结束 
     {
          for(i=m;ht[i].left && ht[i].right; position++) //在Huffman树中查找左右子树为空 ，以构造一个Huffman编码 
          {
              if(code[position]=='0') //编码位为0 
                  i=ht[i].left; //处理左子树 
              else //编译位为 1 
                  i=ht[i].right; //处理右子树 
          }
          decode[j]=alphabet[i-1]; //得到一个字母
          j++;//处理下一字符 
     }  
     decode[j]='\0'; //字符串结尾 
}


int main()
{
    int i,n=4,m; 
    char test[]="DBDBDABDCDADBDADBDADACDBDBD";
    char code[100],code1[100];
    char alphabet[]={'A','B','C','D'}; //4个字符
    int w[]={5,7,2,13} ;//4个字符的权重 
    HuffmanTree *ht;
    HuffmanCode *hc;    
    m=2*n-1;    
    ht=(HuffmanTree *)malloc((m+1)*sizeof(HuffmanTree)); //申请内存，保存赫夫曼树 
    if(!ht)
    {
        printf("内存分配失败！\n");
        exit(0);    
    }
    hc=(HuffmanCode *)malloc(n*sizeof(char*));
    if(!hc)
    {
        printf("内存分配失败！\n");
        exit(0);    
    }
    
    CreateTree(ht,n,w); //创建赫夫曼树 
    HuffmanCoding(ht,n,hc); //根据赫夫曼树生成赫夫曼编码 
    for(i=1;i<=n;i++) //循环输出赫夫曼编码 
        printf("字母:%c,权重:%d,编码为 %s\n",alphabet[i-1],ht[i].weight,hc[i-1]);
    
    Encode(hc,alphabet,test,code); //根据赫夫曼编码生成编码字符串 
    printf("\n字符串:\n%s\n转换后为:\n%s\n",test,code); 
    
    Decode(ht,n,code,alphabet,code1); //根据编码字符串生成解码后的字符串 
    printf("\n编码:\n%s\n转换后为:\n%s\n",code,code1); 
    getch();
    return 0;
}


