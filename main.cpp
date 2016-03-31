#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "resource.h"

#define DEGREE *3.14/180
#define ORDER 4
//#define FEED(x) x.pos=1; x.value=1;
//#define FEED0(x) x.pos=0; x.value=0;
#define SIZE 25
#define SEED 2

void OnCreate(HWND Window_Handle);
void OnPaint(HWND Window_Handle);
void OnTimer(HWND Window_Handle);
int OnMouseMove(HWND Window_Handle,int Msg,int x,int y);
void OnMLDown(HWND hwnd,int x,int y);
void OnMLUp(HWND);
void ChangeOri(HWND hwnd,int direction);
void Display(HWND hwnd);
int TurnsLeft(void);
void OnDestroy(HWND);

typedef struct point_struct
{
    int x,y;

} point;

enum dir{

Right,Left,Up,Down

};

typedef struct ga
{
    int pos,value;

} game;


void FEED(game *x)
{
    x->pos=1;
    x->value=SEED;
}

void FEED0(game *x)
{
    x->pos=0;
    x->value=0;
}


/*
    HBITMAP hbmp_box[3];
    HDC hmemdc_box[3];
    HGDIOBJ holdbmp_box[3];
    HINSTANCE hInit;
*/



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
//    hInit=hThisInstance;


    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int res;
    extern int finish;
//    extern HWND HWINDOW;
//    HWINDOW=hwnd;
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
             OnDestroy(hwnd);     /* send a WM_QUIT to the message queue */
         //   KillTimer(hwnd,0);

            break;
        case WM_CREATE:
            OnCreate(hwnd);
            break;
        case WM_PAINT:
     //       Display(hwnd);
            OnPaint(hwnd);
            break;
        case WM_LBUTTONDOWN:
            if(finish!=2)
            OnMLDown(hwnd,LOWORD(lParam),HIWORD(lParam));
            break;
        case WM_LBUTTONUP:
            if(finish!=2)
            OnMLUp(hwnd);
            break;
        case WM_MOUSEMOVE:
            if(finish==2)
                break;
            res=OnMouseMove(hwnd,wParam,LOWORD(lParam),HIWORD(lParam));
            if(res==-1)
                break;
            else
                ChangeOri(hwnd,res);
            break;
   //     case WM_TIMER:
   //         OnTimer(hwnd);
   //         break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void OnDestroy(HWND hwnd)
{
  /*  for(int i=0;i<3;i++)
    {
        SelectObject ( hmemdc_box[i], holdbmp_box[i] ) ;
        DeleteDC ( hmemdc_box[i] ) ;
        DeleteObject ( hbmp_box[i] ) ;
    }
  */  PostQuitMessage (0);
}

int CalDir(int x1,int y1,int x2,int y2)
{
    float xd1,yd1,xd2,yd2;

    float slope;

    xd1=x1*cos(45 DEGREE)-y1*cos(45 DEGREE); // x' = x cos f - y sin f
    xd2=x2*cos(45 DEGREE)-y2*cos(45 DEGREE);

    yd1=y1*cos(45 DEGREE)+x1*cos(45 DEGREE);  //y' = y cos f + x sin f
    yd2=y2*cos(45 DEGREE)+x2*cos(45 DEGREE);

    slope=(yd2-yd1)/(xd2-xd1);

  //  printf("%d,%d --> %f,%f",x1,y1,xd1,yd1);
    if(slope>0)
        if(yd2>yd1)
            return Right;
        else
            return Left;
    else
        if(yd2>yd1)
            return Down;
        else
            return Up;
    return 0;
}

int OnMouseMove ( HWND hWnd, int flags, int x, int y )
{
 HDC hdc ;
 extern point starting,ending;
 static int value;
 int ans=-1;
 if ( flags == MK_LBUTTON )  /* is left mouse button depressed */
 {
     value=1;
  hdc = GetDC ( hWnd ) ;
  ending.x = x ;
  ending.y = y ;
 // MoveToEx ( hdc, starting.x, starting.y, NULL ) ;
 // LineTo ( hdc, ending.x, ending.y ) ;

  ReleaseDC ( hWnd, hdc ) ;

 // x1 = x2 ;
 // y1 = y2 ;
 }
 else if(value==1)
    value=0,ans=CalDir(starting.x,starting.y,ending.x,ending.y);//printf("%d %d ---> %d %d\n",starting.x,starting.y,ending.x,ending.y);

 return ans;
}

void OnMLDown ( HWND hWnd, int x, int y )
{
  SetCapture ( hWnd ) ;
  extern point starting;
  starting.x = x ;
  starting.y = y ;
}


void OnMLUp(HWND hWnd)
{
    ReleaseCapture();
}

void OnCreate(HWND hwnd)
{
    extern game arr[ORDER][ORDER];
    //PlaySound (TEXT ("HelloWin.wav"), NULL, SND_FILENAME | SND_ASYNC) ;
    FEED(&arr[0][0]);
}

void OnPaint(HWND hwnd)
{

    HDC hdc;
    PAINTSTRUCT ps;
    HFONT hfont ;
    LOGFONT f = { 0 } ;
    HGDIOBJ holdfont ;
    char font[]="Times New Roman",pr[]="Go";
    extern game arr[ORDER][ORDER];
    int i,j;


    hdc=BeginPaint(hwnd,&ps);

    Rectangle(hdc,0,0,8*SIZE,8*SIZE);
    for(i=0;i<ORDER;i++)
    {
        for(j=0;j<ORDER;j++)
        {
            MoveToEx(hdc,i*2*SIZE,0,NULL);
            LineTo(hdc,i*2*SIZE,8*SIZE);

            MoveToEx(hdc,0,j*2*SIZE,NULL);
            LineTo(hdc,8*SIZE,j*2*SIZE);


        }
    }
    strcpy(f.lfFaceName,font);
    f.lfHeight = SIZE ;  /* font height */
  f.lfItalic = 0 ;  /* italic */

  hfont = CreateFontIndirect ( &f ) ;
  holdfont = SelectObject ( hdc, hfont ) ;

  SetTextColor ( hdc, RGB ( 0, 0, 255 ) ) ;




    for(i=0;i<ORDER;i++)
    {
        for(j=0;j<ORDER;j++)
        {
            itoa(arr[i][j].value,pr,10);
            if(arr[i][j].value==0)
                continue;
            TextOut ( hdc, j*2*SIZE+SIZE/2, i*2*SIZE+SIZE/2, pr , strlen(pr) ) ;
        }
    }


    Rectangle(hdc,8*SIZE+40,SIZE+SIZE/2,8*SIZE+275,2*SIZE+SIZE/2+120);
    TextOut ( hdc, 8*SIZE+100, 2*SIZE, "Score Card" , 10 ) ;
    TextOut (hdc,8*SIZE+100+50,2*SIZE+30,"0",1);




    SelectObject ( hdc, holdfont ) ;
    DeleteObject ( hfont ) ;



    EndPaint(hwnd,&ps);

    //Display(hwnd);
}



void Display(HWND hwnd)
{
    HDC hdc;
    HFONT hfont ;
    LOGFONT f = { 0 } ;
    HGDIOBJ holdfont ;




    char font[]="Times New Roman",cScore[7],pr[6];
    extern game arr[ORDER][ORDER];
    extern int score;
    extern int finish;
    int i,j;


    hdc=GetDC(hwnd);



//Brushes for boxes
/*
    hbmp_box[0]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX1 ) ) ;
    hbmp_box[1]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX2 ) ) ;
    hbmp_box[2]=LoadBitmap ( hInit, MAKEINTRESOURCE ( IDB_BOX3  ) ) ;

    for(int i=0;i<3;i++)
    {
        hmemdc_box[i] = CreateCompatibleDC ( hdc ) ;
        holdbmp_box[i] = SelectObject ( hmemdc_box[i], hbmp_box[i] ) ;
    }

*/
//Brushes ended




    Rectangle(hdc,0,0,8*SIZE,8*SIZE);
for(i=0;i<ORDER;i++)
    {
        for(j=0;j<ORDER;j++)
        {
            MoveToEx(hdc,i*2*SIZE,0,NULL);
            LineTo(hdc,i*2*SIZE,8*SIZE);

            MoveToEx(hdc,0,j*2*SIZE,NULL);
            LineTo(hdc,8*SIZE,j*2*SIZE);


        }
    }
    strcpy(f.lfFaceName,font);
    f.lfHeight = SIZE ;  /* font height */
  f.lfItalic = 0 ;  /* italic */

  hfont = CreateFontIndirect ( &f ) ;
  holdfont = SelectObject ( hdc, hfont ) ;

 // BitBlt ( hdc, 0, 0,2*SIZE ,2*SIZE , hmemdc_box[0], 0, 0, SRCCOPY );

  SetTextColor ( hdc, RGB ( 0, 0, 255 ) ) ;




    for(i=0;i<ORDER;i++)
    {
        for(j=0;j<ORDER;j++)
        {
            itoa(arr[i][j].value,pr,10);
            if(arr[i][j].value==0)
                continue;
            TextOut ( hdc, j*2*SIZE+SIZE/2, i*2*SIZE+SIZE/2, pr , strlen(pr) ) ;
        }
    }
    /*        printf("%3d ",arr[i][j].value);
        printf("\n");
    }
    printf("\n");
*/
    itoa(score,cScore,10);
Rectangle(hdc,8*SIZE+40,SIZE+SIZE/2,8*SIZE+275,2*SIZE+SIZE/2+120);
TextOut ( hdc, 8*SIZE+100, 2*SIZE, "Score Card" , 10 ) ;
TextOut (hdc,8*SIZE+100+50,2*SIZE+30,cScore,strlen(cScore));
if(finish==1)
    TextOut (hdc,8*SIZE+100,2*SIZE+70,"Game OVER",9),finish=2;



    SelectObject ( hdc, holdfont ) ;
    DeleteObject ( hfont ) ;


    //    Rectangle(hdc, 200,200,300,300);

    ReleaseDC(hwnd,hdc);

}


int Insert(int di)
{
    int i,rn;
    extern game arr[ORDER][ORDER];
    srand(time(NULL));
    rn=rand()%ORDER;
    switch(di)
    {
        case Right:
            if(arr[rn][0].pos==0){
                FEED(&arr[rn][0]);//arr[rn][0]={1,1};
                return 0;
            }
            for(i=0;i<ORDER;i++)
                if(arr[i][0].pos==0)
                {
                    FEED(&arr[i][0]);//={1,1};
                    return 0;
                }
        case Left:
            if(arr[rn][ORDER-1].pos==0){
                FEED(&arr[rn][ORDER-1]);//={1,1};
                return 0;
            }
            for(i=0;i<ORDER;i++)
                if(arr[i][ORDER-1].pos==0)
                {
                    FEED(&arr[i][ORDER-1]);//={1,1};
                    return 0;
                }
        case Up:
            if(arr[ORDER-1][rn].pos==0){
                FEED(&arr[ORDER-1][rn]);//={1,1};
                return 0;
            }
            for(i=0;i<ORDER;i++)
                if(arr[ORDER-1][i].pos==0)
                {
                    FEED(&arr[ORDER-1][i]);//={1,1};
                    return 0;
                }
        case Down:
            if(arr[0][rn].pos==0){
                FEED(&arr[0][rn]);//={1,1};
                return 0;
            }
            for(i=0;i<ORDER;i++)
                if(arr[0][i].pos==0)
                {
                    FEED(&arr[0][i]);//={1,1};
                    return 0;
                }

    }

    return 1;
}

int PlacesLeft()
{
    int i,j;
    extern game arr[ORDER][ORDER];
    for(i=0;i<ORDER;i++)
        for(j=0;j<ORDER;j++)
            if(arr[i][j].pos==0)
                return 1;
    return 0;
}

void ChangeOri(HWND hwnd,int di)
{
    extern game arr[ORDER][ORDER];
    extern int finish;
    extern int score;
//    system("cls");
    Display(hwnd);
    int i,j,k,l,last;
    switch (di)
    {
        case Right:
            for(i=0;i<ORDER;i++)
                for(j=ORDER-1;j>=0;j--)
            {
                if(arr[i][j].pos==0)
                    continue;
                else{
                    for(k=j,l=0,last=-1;k>=0;k--,l++){                      //Movement
                        if(arr[i][k].pos==0){
                            l--;continue;}
                        if(arr[i][k].value==arr[i][ORDER-1-l+1].value&&l>0&&last!=ORDER-1-l+1)//&&arr[i][k].pos*arr[i][ORDER-1-l+1].pos!=0)
                        {
                             l--;
                            arr[i][k].value*=2;
                            score+=arr[i][k].value;
                            last=ORDER-1-l;
                  //          rep++;
                        }
                        arr[i][ORDER-1-(l)]=arr[i][k];

                    }
             //       if(i==3) printf(" l=%d ",l);
                    for(k=0;k<ORDER-l;k++)//((l>ORDER/2)?ORDER-l:l);k++)   //Deletion
                        FEED0(&arr[i][k]);//={0,0};
                 break;
                 }
            }
            break;
        case Left:
            for(i=0;i<ORDER;i++)
                for(j=0;j<ORDER;j++)
            {
                if(arr[i][j].pos==0)
                    continue;
                else{
                    for(k=j,l=0,last=-1;k<ORDER;k++,l++){                      //Movement
                        if(arr[i][k].pos==0){
                            l--;continue;}
                    //    arr[i][(l)]=arr[i][k];

                        if(arr[i][k].value==arr[i][l-1].value&&l>0&&last!=l-1)//&&arr[i][k].pos*arr[i][ORDER-1-l+1].pos!=0)
                        {
                             l--;
                            arr[i][k].value*=2;
                            score+=arr[i][k].value;
                            last=l;
                          //  rep++;
                        }
                        arr[i][(l)]=arr[i][k];

                    }
             //       if(i==3) printf(" l=%d ",l);
                    for(k=ORDER-1;k>l-1;k--)//((l>ORDER/2)?ORDER-l:l);k++)   //Deletion
                        FEED0(&arr[i][k]);//={0,0};
                 break;
                 }
            }
            break;
        case Down:
            for(j=0;j<ORDER;j++)
                for(i=ORDER-1;i>=0;i--)
            {
                if(arr[i][j].pos==0)
                    continue;
                else{
                    for(k=i,l=0,last=-1;k>=0;k--,l++){                      //Movement
                        if(arr[k][j].pos==0){
                            l--;continue;}

                        if(arr[k][j].value==arr[ORDER-1-l+1][j].value&&l>0&&last!=ORDER-1-l+1)//&&arr[i][k].pos*arr[i][ORDER-1-l+1].pos!=0)
                        {
                             l--;
                            arr[k][j].value*=2;
                            score+=arr[k][j].value;
                            last=ORDER-1-l;
                  //          rep++;
                        }



                        arr[ORDER-1-(l)][j]=arr[k][j];

                    }
             //       if(i==3) printf(" l=%d ",l);
                    for(k=0;k<ORDER-l;k++)//((l>ORDER/2)?ORDER-l:l);k++)   //Deletion
                        FEED0(&arr[k][j]);//={0,0};
                 break;
                 }
            }
            break;
        case Up:
            for(j=0;j<ORDER;j++)
                for(i=0;i<ORDER;i++)
            {
                if(arr[i][j].pos==0)
                    continue;
                else{
                    for(k=i,l=0,last=-1;k<ORDER;k++,l++){                      //Movement
                        if(arr[k][j].pos==0){
                            l--;continue;}

                        if(arr[k][j].value==arr[l-1][j].value&&l>0&&last!=l-1)//&&arr[i][k].pos*arr[i][ORDER-1-l+1].pos!=0)
                        {
                             l--;
                            arr[k][j].value*=2;
                            score+=arr[k][j].value;
                            last=l;
                          //  rep++;
                        }

                        arr[(l)][j]=arr[k][j];

                    }
             //       if(i==3) printf(" l=%d ",l);
                    for(k=ORDER-1;k>l-1;k--)//((l>ORDER/2)?ORDER-l:l);k++)   //Deletion
                        FEED0(&arr[k][j]);//={0,0};
                 break;
                 }
            }
            break;
    }

    if(Insert(di)==1)
        if(TurnsLeft()==0)
            finish=1;

    if(PlacesLeft()==0&&TurnsLeft()==0)
        finish=1;

    Display(hwnd);

//    printf("\n");
}

int TurnsLeft()
{
    int i,j;
    extern game arr[ORDER][ORDER];
    for(i=0;i<ORDER;i++)
        for(j=0;j<ORDER;j++)
    {
        if(j<ORDER-1&&arr[i][j].value==arr[i][j+1].value)
            return 1;
        if(i<ORDER-1&&arr[i][j].value==arr[i+1][j].value)
            return 1;
    }
    return 0;
}




int l_pressed;
point starting,ending;
game arr[ORDER][ORDER]={0};
int finish=0,score=0;
//HWND HWINDOW;
