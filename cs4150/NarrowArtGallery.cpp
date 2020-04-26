#include <iostream> 
#include <climits>
#include <stdio.h>

using namespace std;

int dp[210][3][210],k,n,g[210][2];

int work()
{
    for(int i=0;i<3;i++)
        fill(dp[0][i],dp[0][i]+k+1,INT_MIN);

    /*
    printf("\n");
    for(int i=0;i<n;i++)
        for(int j=0;j<3;j++)
            for(int a=0;a<n;a++)
                printf("DP[%d][%d][%d]: %d\n", i, j, a, dp[i][j][a]);
    */

    printf("\n");
    dp[0][0][0]=g[0][0]+g[0][1];
    // printf("\ndp[0][0][0]: %d\n", dp[0][0][0]);
    // printf("g[0][1]: %d\n", g[0][1]);
    // printf("g[0][0]: %d\n", g[0][0]);
    dp[0][1][1]=g[0][1];
    //printf("dp[0][1][1]: %d\n", dp[0][1][1]);
    dp[0][2][1]=g[0][0];
    //printf("dp[0][2][1]: %d\n", dp[0][2][1]);


    printf("\n");
    /*
    // Reads numbers
        for(int i=0;i<n;i++)
            for(int j=0;j<2;j++)
                printf("g[%d][%d]: %d\n", i, j, g[i][j]);
    */

    for(int i=1;i<n;i++)
        for(int j=0;j<=k;j++)
        {
            printf("CHECK%d>>>>>>>>>>>>>>>\n", i);
            // printf("dp[i-1][1][j]: %d\n", dp[i-1][1][j]);
            // printf("dp[i-1][2][j]: %d\n", dp[i-1][2][j]);
            // printf("max(dp[i-1][1][j],dp[i-1][2][j]): %d\n", max(dp[i-1][1][j],dp[i-1][2][j]));
            // printf("dp[i-1][0][j]: %d\n", dp[i-1][0][j]);
            // printf("max(dp[i-1][0][j],  max(dp[i-1][1][j],dp[i-1][2][j])): %d\n", max(dp[i-1][0][j],  max(dp[i-1][1][j],dp[i-1][2][j])));
            // printf("g[i][0]+g[i][1]: %d\n", g[i][0]+g[i][1]);

            dp[i][0][j]= max(dp[i-1][0][j],  max(dp[i-1][1][j],dp[i-1][2][j]))  +g[i][0]+g[i][1];
            printf("dp[%d][0][%d] >>>>>>>>>>>>: %d\n", i, j, dp[i][0][j]);

            if(j==0) {
                dp[i][1][j]=dp[i][2][j]=INT_MIN;
                printf("CHECK2\n");
                printf("p[%d][1][%d] >>>>>>>>>>>>: %d\n", i, j, dp[i][1][j]);
                printf("dp[%d][2][%d] >>>>>>>>>>>>: %d\n", i, j, dp[i][2][j]);
            }
            else
            {
                // printf("CHECK3\n");
                // printf("dp[i-1][1][j-1]: %d\n", dp[i-1][1][j-1]);
                // printf("dp[i-1][0][j-1]: %d\n", dp[i-1][0][j-1]);
                // printf("max(dp[i-1][0][j-1],dp[i-1][1][j-1]): %d\n", max(dp[i-1][0][j-1],dp[i-1][1][j-1]));
                // printf("g[i][1]: %d\n", g[i][1]);

                dp[i][1][j]=max(dp[i-1][0][j-1],dp[i-1][1][j-1])+g[i][1];
                printf("dp[%d][1][%d] >>>>>>>>>>>>: %d\n", i, j, dp[i][1][j]);

                // printf("CHECK4\n");
                // printf("dp[i-1][2][j-1]: %d\n", dp[i-1][2][j-1]);
                // printf("dp[i-1][0][j-1]: %d\n", dp[i-1][0][j-1]);
                // printf("max(dp[i-1][0][j-1],dp[i-1][2][j-1]): %d\n", max(dp[i-1][0][j-1],dp[i-1][2][j-1]));
                // printf("g[i][0]: %d\n", g[i][0]);

                dp[i][2][j]=max(dp[i-1][0][j-1],dp[i-1][2][j-1])+g[i][0];
                printf("dp[%d][2][%d] >>>>>>>>>>>>: %d\n", i, j, dp[i][2][j]);
            }
            printf("\n");
            printf("\n");
        }

    printf("RESULTS\n");
    printf("dp[n-1][2][k]: %d\n", dp[n-1][2][k]);
    printf("dp[n-1][1][k]: %d\n", dp[n-1][1][k]);
    printf("dp[n-1][0][k]: %d\n", dp[n-1][0][k]);
    printf("max(dp[n-1][0][k],max(dp[n-1][1][k],dp[n-1][2][k])): %d\n", max(dp[n-1][0][k],max(dp[n-1][1][k],dp[n-1][2][k])));

    return max(dp[n-1][0][k],max(dp[n-1][1][k],dp[n-1][2][k]));
}

int main()
{
    while(cin>>n>>k) // Read n and k
    {
        // If both zero do nothing
        if(n==0&&k==0)  break;

        // Reads numbers
        for(int i=0;i<n;i++)
            for(int j=0;j<2;j++)
                cin>>g[i][j];

        cout<<work()<<endl;
    }
}