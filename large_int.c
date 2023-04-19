#include<stdio.h>
#include<string.h>
#include<stdlib.h>


/* karatsuba's algo:
you have 2 numbers x,y
you halve each of them
a = 1st halve of x
b = 2nd halve of x
c = 1st halve of y
d = 2nd halve of y
n = number of digits of the biggest number if it's odd then it is decremented by 1
id n is odd then a and c take a bigger part than b and d respectively.
if number of digits of both numbers are different we shift as in concat zeros to the right of the lowest by the difference of the number of digits between them
1) kar(a, c) ---> f
2) kar(b, d) ---> s
3) kar(a+b, c+d) -> t
4) f - s - t --> Fourth
5) shiftleft(f,n) + s + shiftleft(Fourth, ceil(n/2) ) 

*/
typedef unsigned char u8;


void trim_left_zeros(u8* x)
{
    int i;
    for(i = 0; x[i] == '0'; i++){}
    u8* temp = (u8*) malloc(sizeof(u8)* strlen(x) + 1);
    strcpy(temp,&x[i]);
    strcpy(x,temp);
    free(temp);
    if(strlen(x) == 0)
    {
        strcpy(x,"0");
    }
}

void init_zeros(u8* x, int len)
{
    strcpy(x,"0");
    for(int i = 1; i < len; i++)
    {
        strcat(x,"0");
    }
}


void shift_r(u8** x, int amount)
{

    u8* zeros = (u8*)malloc(sizeof(u8)* (amount +( strlen(*x)) + 10));
    *x = (u8*)realloc(*x, (amount + (strlen(*x)) + 10));

    if(zeros == NULL)
    {
        printf("not allocating\n");
        return ;
    }
    init_zeros(zeros, amount);
    strcat(zeros,*x);

    strcpy(*x,zeros);
    free(zeros);

}

void shift_l(u8** x, int amount)
{

    u8* zeros = (u8*)malloc(sizeof(u8)* (amount +( strlen(*x)) + 10));
    *x = (u8*)realloc(*x, (amount + (strlen(*x)) + 10));
    if(zeros == NULL)
    {
        printf("not allocating\n");
        return ;
    }
    init_zeros(zeros, amount);
    strcat(*x,zeros);
    free(zeros);

}

u8* add(u8* a, u8* b)
{
    u8* x = (u8*) malloc(sizeof(u8) * (strlen(a) + 1));
    u8* y = (u8*) malloc(sizeof(u8) * (strlen(b) + 1));
    strcpy(x,a);
    strcpy(y,b);
    int diff_num_of_digits = 0;
    if(strlen(x) != strlen(y))
    {
        diff_num_of_digits = strlen(x) - strlen(y);
        if(diff_num_of_digits < 0)
        {
            shift_r(&x,diff_num_of_digits * -1 );
        }
        else
        {
           shift_r(&y,diff_num_of_digits); 
        }
    }
    shift_r(&y, 1);
    shift_r(&x, 1);

    int n = strlen(x);
    u8 result[n + 1];
    result[n] = '\0';

    u8 carry = 0;
    int i;
    for (i = n - 1; i >= 0 ; i--)
    {
        result[i] = carry + (x[i] - 48) + (y[i] - 48);
        carry = 0;
        if(result[i] > 9)
        {
            carry = result[i] / 10;
            result[i] = (result[i] % 10) + 48;
        }
        else
        {
            result[i] += 48;
        }
    }

   
    u8* z = (u8*) malloc(sizeof(u8) * (n + 1) );
    if(result[0] == '0')
    {
        strcpy(z,&result[1]);
        free(x);
        free(y);
        return z;
    }
    strcpy(z, result);
    free(x);
    free(y);
    return z;
    
}


void borrow(u8* x,u8* res ,int index)
{
    if(res[index - 1] == 0)
    {
        borrow(x,res, index - 1);
        res[index - 1] -= 1;
        res[index] += 10;
        x[index] += 10;
        return;
        
    }
    else
    {
        res[index - 1] -= 1;
        res[index] += 10;
        x[index] += 10;
    }
    return;
}

u8* sub(u8* temp_bigger, u8* temp_smaller)
{
    u8* smaller = (u8*) malloc(sizeof(u8)* (strlen(temp_smaller) + 1));
    u8* bigger = (u8*) malloc(sizeof(u8)* (strlen(temp_bigger) + 1));
    strcpy(bigger,temp_bigger);
    strcpy(smaller, temp_smaller);
    u8* res;
    if(strcmp(bigger,smaller) == 0)
    {
        res = (u8*)malloc(sizeof(u8)*2);
        strcpy(res, "0");
        free(bigger);
        free(smaller);
        return res;
    }
    if(strcmp(smaller,"0") == 0)
    {
        res = (u8*)malloc(sizeof(u8)* strlen(bigger) +1 );
        strcpy(res, bigger);
        free(bigger);
        free(smaller);
        return res;
    }
    int diff_num_of_digits = 0;
    if(strlen(bigger) > strlen(smaller))
    {
        diff_num_of_digits = strlen(bigger) - strlen(smaller);
        shift_r(&smaller, diff_num_of_digits);
    }
    int n = strlen(bigger);
    u8 result[n+1];
    result[n] = '\0';

    for(int i =0; i < n; i++)
    {
        result[i] = 0;
    }
    
    for(int i = 0; i < n; i++)
    {
        if(bigger[i] >= smaller[i])
        {
            result[i] = bigger[i] - smaller[i];
        }
        else
        {
            borrow(bigger, result, i);
            result[i] = bigger[i] - smaller[i];
        }
        
    } 
    res = (u8*)malloc(sizeof(u8)*(n+1));
    for(int i =0; i < n; i++)
    result[i]+=48;
    trim_left_zeros(smaller);
    strcpy(res, result);
    trim_left_zeros(res);
    free(bigger);
    free(smaller);
    return res;
}

u8* kar_mul(u8* a, u8* b)
{
    
    u8* x = (u8*) malloc(sizeof(u8) * (strlen(a) + 1));
    u8* y = (u8*) malloc(sizeof(u8) * (strlen(b) + 1));
    strcpy(x,a);
    strcpy(y,b);
    static int counter = 0;
    counter++;

    if(strcmp(x,"0") == 0 || strcmp(y,"0") == 0)
    {

        u8* result = (u8*)malloc(sizeof(u8) * 2); 
        strcpy(result,"0");

        free(x);
        free(y);
        return result;

    }


    int x_len, y_len,n1;
    x_len = strlen(x);
    y_len = strlen(y);
    n1 = x_len + y_len;
    if(x_len == 1 && y_len == 1)
    {
        u8 mul = (x[0] - 48) * (y[0] - 48);
        int n = mul >= 10? 3:2;
        u8* result = (u8*)malloc(sizeof(u8) * n);
        result[n-1] = '\0';
        for(int i = n-2; i >= 0; i--)
        {
            result[i] = (mul % 10) + 48;
            mul = mul / 10;
        }
        free(x);
        free(y);
        return result;
    }
    else
    {
        int n;
        int amount_of_shifting = x_len - y_len;
        n = x_len;
        if(amount_of_shifting > 0)
        {
            n = x_len;
            shift_r(&y, amount_of_shifting);
        }
        else if(amount_of_shifting < 0)
        {
            n = y_len;
            shift_r(&x, amount_of_shifting * -1);
        }
        u8* a,*b,*c,*d;
        a = (u8*)malloc(sizeof(u8) * ((n/2) + 2));
        b = (u8*)malloc(sizeof(u8) * ((n/2) + 2));
        c = (u8*)malloc(sizeof(u8) * ((n/2) + 2));
        d = (u8*)malloc(sizeof(u8) * ((n/2) + 2));

        if(n%2 != 0)
        {
            
            strncpy(a,x,(n/2) + 1  );
            a[(n/2) + 1] = '\0'; 
            strcpy(b,&x[(n/2)+1]);
            strncpy(c,y,(n/2) + 1  ); 
            c[(n/2) + 1] = '\0';
            strcpy(d,&y[(n/2)+1]);
            n--;
        }
        else
        {
            strncpy(a,x,n/2);
            a[(n/2)] = '\0'; 
            strcpy(b,&x[n/2]);
            strncpy(c,y,n/2);
            c[(n/2) ] = '\0';
            strcpy(d,&y[n/2]);
        }
        trim_left_zeros(a);
        trim_left_zeros(b);
        trim_left_zeros(c);
        trim_left_zeros(d);

        u8* first_step, *second_step, *third_step, *fourth_step, *final_result, *add_ab, *add_cd, *add_second_first, *term1;
        int a_len, b_len, c_len, d_len;

        add_ab = add(a,b);
        add_cd = add(c,d);

        first_step = kar_mul(a, c);

        second_step = kar_mul(b, d);
        free(a);
        free(b);
        free(c);
        free(d);

        third_step = kar_mul( add_ab, add_cd);
        free(add_ab);
        free(add_cd);

        add_second_first = add(first_step,second_step);

        fourth_step = sub(third_step, add_second_first);
        
        shift_l(&first_step,n);
        shift_l(&fourth_step, n/2);

        term1 = add(first_step,second_step);

        final_result = add(term1, fourth_step);
        free(term1);
        trim_left_zeros(final_result);


        free(first_step);
        free(second_step);
        free(third_step);
        free(fourth_step);
        free(add_second_first);
        free(x);
        free(y);
        return final_result;
    }
}

int main()
{
    u8* x = (u8*)malloc(sizeof(u8)*600);
    u8* y = (u8*)malloc(sizeof(u8)*600);

    printf("enter the first number: ");
    scanf("%s",x);
    printf("enter the second number: ");
    scanf("%s",y);


    u8* z = kar_mul(x,y);

    printf("x = %s\n",x);
    printf("y = %s\n",y);
    printf("z = %s\n",z);

    free(z);
    free(x);
    free(y);

    return 0;
}