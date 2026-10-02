#include <iostream>
#include <string>
using namespace std;

void clearScreen(){ system("cls"); }
void waitEnter(){ cout<<"\nPress Enter..."; cin.ignore(); cin.get(); }

// ===================== ARRAY MODULE (UPDATED) =====================
struct ArrayStudents{
    string name[100];
    int roll[100];
    int size;
};

// Display all students
void printArray(ArrayStudents &arr){
    if(arr.size==0){ cout<<"ARRAY EMPTY\n"; return; }
    for(int i=0;i<arr.size;i++)
        cout<<"["<<arr.name[i]<<"|"<<arr.roll[i]<<"] ";
    cout<<"\n";
}

// Insert at beginning
void insertBegArray(ArrayStudents &arr, string n, int r){
    for(int i=arr.size;i>0;i--){
        arr.name[i]=arr.name[i-1];
        arr.roll[i]=arr.roll[i-1];
    }
    arr.name[0]=n;
    arr.roll[0]=r;
    arr.size++;
}

// Insert at end
void insertEndArray(ArrayStudents &arr, string n, int r){
    arr.name[arr.size]=n;
    arr.roll[arr.size]=r;
    arr.size++;
}

// Insert at specific position (0-based)
void insertPosArray(ArrayStudents &arr, string n, int r, int pos){
    if(pos<0) pos=0;
    if(pos>arr.size) pos=arr.size;
    for(int i=arr.size;i>pos;i--){
        arr.name[i]=arr.name[i-1];
        arr.roll[i]=arr.roll[i-1];
    }
    arr.name[pos]=n;
    arr.roll[pos]=r;
    arr.size++;
}

// Array menu
void ArrayMenu(){
    ArrayStudents arr;
    arr.size=3; // preloaded students
    arr.name[0]="Ali"; arr.roll[0]=101;
    arr.name[1]="Sara"; arr.roll[1]=102;
    arr.name[2]="Omar"; arr.roll[2]=103;

    int ch;
    do{
        clearScreen();
        cout<<"=== ARRAY MODULE ===\nCurrent Array:\n";
        printArray(arr);
        cout<<"1.Insert at Beginning\n2.Insert at End\n3.Insert at Position\n4.Traverse\n0.Back\nEnter: ";
        cin>>ch;
        if(ch==1){
            string n; int r;
            cout<<"Name: "; cin>>n;
            cout<<"Roll: "; cin>>r;
            cout<<"\nBefore:\n"; printArray(arr);
            insertBegArray(arr,n,r);
            cout<<"After:\n"; printArray(arr);
            cin.ignore(); waitEnter();
        }else if(ch==2){
            string n; int r;
            cout<<"Name: "; cin>>n;
            cout<<"Roll: "; cin>>r;
            cout<<"\nBefore:\n"; printArray(arr);
            insertEndArray(arr,n,r);
            cout<<"After:\n"; printArray(arr);
            cin.ignore(); waitEnter();
        }else if(ch==3){
            string n; int r,pos;
            cout<<"Name: "; cin>>n;
            cout<<"Roll: "; cin>>r;
            cout<<"Position (0-based): "; cin>>pos;
            cout<<"\nBefore:\n"; printArray(arr);
            insertPosArray(arr,n,r,pos);
            cout<<"After:\n"; printArray(arr);
            cin.ignore(); waitEnter();
        }else if(ch==4){
            cout<<"\nTraversing Array:\n";
            printArray(arr);
            cin.ignore(); waitEnter();
        }
    }while(ch!=0);
}

// ===================== STUDENT STRUCT =====================
struct Student{
    int roll; string name;
    Student* next;
};

void printList(Student* head){
    if(!head){ cout<<"EMPTY\n"; return; }
    Student* t=head;
    while(t){
        cout<<"["<<t->name<<"|"<<t->roll<<"]";
        if(t->next) cout<<" -> ";
        t=t->next;
    }
    cout<<" -> NULL\n";
}

// ===================== SLL INSERT =====================
Student* insertBeg(Student* head,string n,int r){
    Student* node=new Student;
    node->name=n; node->roll=r; node->next=head;
    return node;
}
Student* insertEnd(Student* head,string n,int r){
    Student* node=new Student;
    node->name=n; node->roll=r; node->next=NULL;
    if(!head) return node;
    Student* t=head;
    while(t->next) t=t->next;
    t->next=node;
    return head;
}
Student* insertPos(Student* head,string n,int r,int pos){
    Student* node=new Student;
    node->name=n; node->roll=r; node->next=NULL;
    if(pos<=1 || !head){ node->next=head; return node; }
    Student* t=head;
    for(int i=1;i<pos-1 && t->next;i++) t=t->next;
    node->next=t->next;
    t->next=node;
    return head;
}

void SLLmenu(Student*& head){
    int ch;
    do{
        clearScreen();
        cout<<"=== SINGLY LINKED LIST ===\nCurrent:\n";
        printList(head);
        cout<<"1.Insert Beg\n2.Insert End\n3.Insert Pos\n4.Display\n0.Back\nEnter: ";
        cin>>ch;
        if(ch==1){
            string n; int r;
            cout<<"Name:"; cin>>n;
            cout<<"Roll:"; cin>>r;
            cout<<"\nBefore:\n"; printList(head);
            head=insertBeg(head,n,r);
            cout<<"After:\n"; printList(head);
            cin.ignore(); waitEnter();
        }
        else if(ch==2){
            string n; int r;
            cout<<"Name:"; cin>>n;
            cout<<"Roll:"; cin>>r;
            cout<<"\nBefore:\n"; printList(head);
            head=insertEnd(head,n,r);
            cout<<"After:\n"; printList(head);
            cin.ignore(); waitEnter();
        }
        else if(ch==3){
            string n; int r,pos;
            cout<<"Name:"; cin>>n;
            cout<<"Roll:"; cin>>r;
            cout<<"Position:"; cin>>pos;
            cout<<"\nBefore:\n"; printList(head);
            head=insertPos(head,n,r,pos);
            cout<<"After:\n"; printList(head);
            cin.ignore(); waitEnter();
        }
        else if(ch==4){
            cout<<"\nDisplay:\n"; printList(head);
            cin.ignore(); waitEnter();
        }
    }while(ch!=0);
}

// ===================== CIRCULAR LL =====================
struct CNode{
    int roll; string name;
    CNode* next;
};

void printCLL(CNode* head){
    if(!head){ cout<<"EMPTY\n"; return; }
    CNode* t=head;
    do{
        cout<<"["<<t->name<<"|"<<t->roll<<"]";
        t=t->next;
        if(t!=head) cout<<" -> ";
    }while(t!=head);
    cout<<" -> (head)\n";
}

CNode* delBeg(CNode* head){
    if(!head) return NULL;
    if(head->next==head){
        cout<<"Deleted: "<<head->name<<"\n";
        delete head; return NULL;
    }
    CNode* last=head;
    while(last->next!=head) last=last->next;
    CNode* del=head;
    head=head->next;
    last->next=head;
    cout<<"Deleted: "<<del->name<<"\n";
    delete del;
    return head;
}

CNode* delEnd(CNode* head){
    if(!head) return NULL;
    if(head->next==head){
        cout<<"Deleted: "<<head->name<<"\n"; delete head; return NULL;
    }
    CNode* t=head,*prev=NULL;
    while(t->next!=head){ prev=t; t=t->next; }
    prev->next=head;
    cout<<"Deleted: "<<t->name<<"\n";
    delete t;
    return head;
}

CNode* delPos(CNode* head,int pos){
    if(!head) return NULL;
    if(pos<=1) return delBeg(head);
    CNode* t=head;
    for(int i=1;i<pos-1 && t->next!=head;i++) t=t->next;
    if(t->next==head) return delBeg(head);
    CNode* del=t->next;
    t->next=del->next;
    cout<<"Deleted: "<<del->name<<"\n";
    delete del;
    return head;
}

void CLLmenu(CNode*& head){
    int ch;
    do{
        clearScreen();
        cout<<"=== CIRCULAR LINKED LIST ===\nCurrent:\n";
        printCLL(head);
        cout<<"1.Delete Beg\n2.Delete End\n3.Delete Pos\n4.Display\n0.Back\nEnter: ";
        cin>>ch;
        if(ch==1){
            cout<<"Before:\n"; printCLL(head);
            head=delBeg(head);
            cout<<"After:\n"; printCLL(head);
            cin.ignore(); waitEnter();
        }
        else if(ch==2){
            cout<<"Before:\n"; printCLL(head);
            head=delEnd(head);
            cout<<"After:\n"; printCLL(head);
            cin.ignore(); waitEnter();
        }
        else if(ch==3){
            int p; cout<<"Position:"; cin>>p;
            cout<<"Before:\n"; printCLL(head);
            head=delPos(head,p);
            cout<<"After:\n"; printCLL(head);
            cin.ignore(); waitEnter();
        }
        else if(ch==4){
            printCLL(head);
            cin.ignore(); waitEnter();
        }
    }while(ch!=0);
}

// ===================== STACK =====================
Student* push(Student* top,string n,int r){
    Student* node=new Student;
    node->name=n; node->roll=r; node->next=top;
    return node;
}
Student* pop(Student* top){
    if(!top){ cout<<"Underflow\n"; return NULL; }
    cout<<"Popped: "<<top->name<<"\n";
    Student* del=top;
    top=top->next;
    delete del;
    return top;
}

void StackMenu(Student*& top){
    int ch;
    do{
        clearScreen();
        cout<<"=== STACK ===\nCurrent:\n";
        printList(top);
        cout<<"1.Push\n2.Pop\n3.Display\n0.Back\nEnter: ";
        cin>>ch;
        if(ch==1){
            string n; int r;
            cout<<"Name:"; cin>>n;
            cout<<"Roll:"; cin>>r;
            cout<<"Before:\n"; printList(top);
            top=push(top,n,r);
            cout<<"After:\n"; printList(top);
            cin.ignore(); waitEnter();
        }
        else if(ch==2){
            cout<<"Before:\n"; printList(top);
            top=pop(top);
            cout<<"After:\n"; printList(top);
            cin.ignore(); waitEnter();
        }
        else if(ch==3){
            printList(top);
            cin.ignore(); waitEnter();
        }
    }while(ch!=0);
}

// ===================== QUEUE =====================
struct QNode{
    int roll; string name;
    QNode* next;
};
QNode *qf=NULL,*qr=NULL;

void enqueue(string n,int r){
    QNode* node=new QNode;
    node->name=n; node->roll=r; node->next=NULL;
    if(!qr){ qf=qr=node; return; }
    qr->next=node;
    qr=node;
}
void dequeueQ(){
    if(!qf){ cout<<"Underflow\n"; return; }
    QNode* del=qf;
    cout<<"Dequeued: "<<del->name<<"\n";
    qf=qf->next;
    if(!qf) qr=NULL;
    delete del;
}
void printQ(){
    if(!qf){ cout<<"EMPTY\n"; return; }
    QNode* t=qf;
    while(t){
        cout<<"["<<t->name<<"|"<<t->roll<<"]";
        if(t->next) cout<<" -> ";
        t=t->next;
    }
    cout<<" -> NULL\n";
}

void QueueMenu(){
    int ch;
    do{
        clearScreen();
        cout<<"=== QUEUE ===\nCurrent:\n";
        printQ();
        cout<<"1.Enqueue\n2.Dequeue\n3.Display\n0.Back\nEnter: ";
        cin>>ch;
        if(ch==1){
            string n; int r;
            cout<<"Name:"; cin>>n;
            cout<<"Roll:"; cin>>r;
            cout<<"Before:\n"; printQ();
            enqueue(n,r);
            cout<<"After:\n"; printQ();
            cin.ignore(); waitEnter();
        }
        else if(ch==2){
            cout<<"Before:\n"; printQ();
            dequeueQ();
            cout<<"After:\n"; printQ();
            cin.ignore(); waitEnter();
        }
        else if(ch==3){
            printQ();
            cin.ignore(); waitEnter();
        }
    }while(ch!=0);
}

// ===================== SORTING (NUMBERS ONLY) =====================
void showNumbers(int r[], int n){
    for(int i=0;i<n;i++)
        cout<<r[i]<<" ";
    cout<<"\n";
}

void bubbleShort(int r[], int n){
    cout<<"Initial: "; showNumbers(r,n);
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(r[j] > r[j+1])
                swap(r[j],r[j+1]);
        }
        cout<<"Pass "<<i+1<<": "; showNumbers(r,n);
    }
}

void selectionShort(int r[], int n){
    cout<<"Initial: "; showNumbers(r,n);
    for(int i=0;i<n-1;i++){
        int min=i;
        for(int j=i+1;j<n;j++)
            if(r[j]<r[min]) min=j;
        swap(r[i],r[min]);
        cout<<"Step "<<i+1<<": "; showNumbers(r,n);
    }
}

void insertionShort(int r[], int n){
    cout<<"Initial: "; showNumbers(r,n);
    for(int i=1;i<n;i++){
        int key=r[i];
        int j=i-1;
        while(j>=0 && r[j]>key){
            r[j+1]=r[j];
            j--;
        }
        r[j+1]=key;
        cout<<"Step "<<i<<": "; showNumbers(r,n);
    }
}

void SortingMenu(){
    int n;
    cout<<"How many numbers to sort? ";
    cin>>n;
    int r[100];
    for(int i=0;i<n;i++){
        cout<<"Number "<<i+1<<": ";
        cin>>r[i];
    }

    int ch;
    do{
        clearScreen();
        cout<<"Current Array: "; showNumbers(r,n);
        cout<<"\n1.Bubble Sort\n2.Selection Sort\n3.Insertion Sort\n0.Back\nEnter: ";
        cin>>ch;

        int temp[100];
        for(int i=0;i<n;i++) temp[i]=r[i];

        if(ch==1){ bubbleShort(temp,n); cin.ignore(); waitEnter(); }
        else if(ch==2){ selectionShort(temp,n); cin.ignore(); waitEnter(); }
        else if(ch==3){ insertionShort(temp,n); cin.ignore(); waitEnter(); }

    }while(ch!=0);
}

// ===================== MAIN =====================
int main(){
    Student* sll=NULL;
    Student* stk=NULL;
    CNode* cll=NULL;

    // preload SLL
    sll=insertEnd(sll,"Ali",101);
    sll=insertEnd(sll,"Sara",102);

    // preload Stack
    stk=push(stk,"Ali",101);
    stk=push(stk,"Sara",102);

    // preload Queue
    enqueue("Ali",101);
    enqueue("Sara",102);

    // preload Circular LL
    CNode* a=new CNode{201,"C_A",NULL};
    CNode* b=new CNode{202,"C_B",NULL};
    CNode* c=new CNode{203,"C_C",NULL};
    a->next=b; b->next=c; c->next=a;
    cll=a;

    int ch;
    do{
        clearScreen();
        cout<<"==== STUDENT RECORD PROJECT ====\n";
        cout<<"1.Array\n";
        cout<<"2.Singly Linked List\n";
        cout<<"3.Circular Linked List\n";
        cout<<"4.Stack\n";
        cout<<"5.Queue\n";
        cout<<"6.Sorting (Numbers Only)\n";
        cout<<"0.Exit\nEnter: ";
        cin>>ch;

        if(ch==1) ArrayMenu();
        else if(ch==2) SLLmenu(sll);
        else if(ch==3) CLLmenu(cll);
        else if(ch==4) StackMenu(stk);
        else if(ch==5) QueueMenu();
        else if(ch==6) SortingMenu();

    }while(ch!=0);

    cout<<"Good luck \n";
    return 0;
}

