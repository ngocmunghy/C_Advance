#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "inc/bt.h"
#include "inc/bc.h"
#include "inc/btr.h"
#include "inc/btree.h"

int main()
{
    int i, n, a;
    long sdt;
    char ten[50];
    BTA * book;
    btinit();
    book = btopn("danhba", 0, 0);
    if (!book) {
        printf("Tao danh ba\n");
	       book = btcrt("danhba", 0, 0);
        printf("Them moi\n");
        sdt = 987906734;
        if ( btins(BTA * book, "Duc", (char*)&sdt, sizeof(long)) )
		      printf("Khong the them Duc\n");
        sdt = 985338237;
        if ( btins(BTA * book, "Huyen", (char*)&sdt, sizeof(long)) )
		      printf("Khong the them Huyen\n");
        sdt = 978359985;
        if ( btins(BTA * book, "Linh", (char*)&sdt, sizeof(long)) )
		      printf("Khong the them Linh\n");
        sdt = 1647197845;
        if ( btins(BTA * book, "Diem", (char*)&sdt, sizeof(long)) )
		      printf("Khong the them Diem\n");
		    sdt = 1687439734;
		    if ( btins(BTA * book, "Kien", (char*)&sdt, sizeof(long)) )
		      printf("Khong the them Kien\n");
    }
	
    do
    {
        printf("1.Them so dien thoai\n2.Tim kiem\n3.In danh sach\n4.Xoa so\n5.Thoat\nChon tu 1-5:");
        scanf("%d",&n);
        while (getchar()!='\n');
        switch(n){
              case 1:
                   printf("Ten:");
                   gets(ten);
                   printf("So dien thoai:");
                   scanf("%d",&sdt);
                   while (getchar()!='\n');
                   btins(book, ten, (char*)&sdt, sizeof(long)); 
                   break;
              case 2:
                   printf("Ten:");
                   gets(ten);
                   if ( btsel(book, ten, (char*)&sdt, sizeof(long), &a) ) 
			               printf("Khong tim thay so cua %s!\n", ten);
                   else 
			               printf("So dien thoai cua %s la %d\n", ten, sdt);
                   break;
              case 3:
                   btsel(book, "", (char*)&sdt, sizeof(long), &a);
		                while ( btseln(book, ten, (char*)&sdt, sizeof(long), &a)==0 ) {
			               printf("%s\t%10d\n", ten, sdt);
                   }	
                   break;
              case 4:
              		printf("nhap vao so dien thoai nguoi can xoa:\n");
              		scanf("%d",&sdt);
              		if(btsel(book, ten, (char*)&sdt, sizeof(long), &a))
              			printf("Khong co so ban muon xoa\n");
              		else{
              			btdel(BTA * book, (char*)&sdt);
              		}

              case 5:
                   btcls(book);
                   break;
          }
     }while(n!=5);
     return 0;
}
