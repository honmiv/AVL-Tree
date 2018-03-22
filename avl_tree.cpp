#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <algorithm>
using namespace std;

struct node // структура - узел
{
    int key;
    struct node* left;
    struct node* right;
    int height;
};

int height(struct node* N) // Возвращает высоту дерева
{
    if (N == NULL)
        return 0;
    return N->height;
}

struct node* newnode(int key)  // функция, создающая новый узел
{
    struct node* node = (struct node*)malloc(sizeof(struct node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

// правый поворот вокург узла y
struct node* rightRotate(struct node* y) 
{
    struct node* x = y->left;
    struct node* T2 = x->right;

    // поворот
    x->right = y;
    y->left = T2;

    // восстановление корректной высоты
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    // Возвращение нового корня
    return x;
}

// левый поворот вокруг узла x
struct node* leftRotate(struct node* x)
{
    struct node* y = x->right;
    struct node* T2 = y->left;

    // поворот
    y->left = x;
    x->right = T2;

    // восстановление корректной высоты
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    return y; // Возвращение нового корня
}

// возвращает баланс-фактор узла
int getBalance(struct node* N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

struct node* balancing(struct node* root)
{
    int balance = getBalance(root); //получение баланс-фактора
 
   if (balance > 1 && getBalance(root->left) >= 0) // малый правый поворота
        return rightRotate(root);
 
    if (balance > 1 && getBalance(root->left) < 0) // большой правый поворот
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
 
    if (balance < -1 && getBalance(root->right) <= 0) // малый левый поворот
        return leftRotate(root);
 
    if (balance < -1 && getBalance(root->right) > 0) // большой правый поворот
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

?

// рекурсивная вставка узла
struct node* insert(struct node* node, int key)
{
    if (node == NULL)
        return(newnode(key));

    if (key < node->key)
        node->left  = insert(node->left, key);
    else
        node->right = insert(node->right, key);

    // восстановление высоты после добавления
    node->height = 1 + max(height(node->left), height(node->right));
	
	return balancing(node);
}

void printtree(node* root) // прямой обход
{
	printf("%d, ", root->key);
	if (root->left != NULL)
		printtree(root->left);
	if (root->right != NULL)
		printtree(root->right);
}

// возвращает минимальный узел в поддереве
struct node*  minValuenode(struct node* node)
{
    struct node* current = node;

    // циклический спуск в левое поддерево
    while (current->left != NULL)
        current = current->left;

    return current;
}

//возвращает корень дерева с удаленным узлом
struct node* deletenode(struct node* root, int key)
{
    // Если дерево пусто возвращаем NULL
    if (root == NULL)
        return root;

    // Если удаляемый ключ меньше ключа корня, то он лежит в левом поддереве
    if ( key < root->key )
        root->left = deletenode(root->left, key);

    // Если удаляемый ключ больше ключа корня, то он лежит в правом поддереве
    else if( key > root->key )
        root->right = deletenode(root->right, key);

    // если ключ равен ключу корня, этот корень необходимо удалить
    else
    {
        // случай узла с 1 и 0 потомками
        if( (root->left == NULL) || (root->right == NULL) )
        {
			// temp - правый или левйы потомок
            struct node* temp = root->left ? root->left : root->right;
            // 0 потомков
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // 1 потомок
             *root = *temp; //содержимое потомка встает на место корня
            free(temp); // освобождается память потомка
        }
        else
        {
            // 2 потомка - поиск минимального в правом поддереве
            struct node* temp = minValuenode(root->right);

            // копируется в корень
            root->key = temp->key;

            // удаляется 
            root->right = deletenode(root->right, temp->key);
        }
    }

    // Если узел удален возвращаем NULL
    //if (root == NULL)
      //return root;
  
	return balancing(root);
}

void printmenu()
{
	printf("1.Добавить узел\n2.Удалить узел\n3.Вывести дерево\n4.Выйти\n");
}




int main()
{
	srand(0);
	setlocale(LC_ALL, "russian");
	int m = 0;
	struct node* root = NULL; //корень дерева
	for (int i = 0; i<15; i++)
	{
		srand(i);
		int k = rand() % 20;
		printf("\nДобавлено: %d \n", k);
		root = insert(root, k);
		printtree(root);
	}

	printf("\n");
	cout << "Нажмите, чтобы продолжить... ";
	getchar();

	while (1)
	{
		system("CLS");
		printmenu();
		cout << "Выберите пункт меню: "; cin >> m;
		if (m == 1)
		{
			int k; cout << "Введите ключ\n"; cin >> k;
			root = insert(root, k);
			printtree(root);
			system("pause");
        }
		if (m == 2)
		{
			int k; cout << "Введите ключ\n"; cin >> k;
			root = deletenode(root, k);
			printtree(root);
			system("pause");
		}
		if (m == 3)
		{
			printtree(root);
			system("pause");
		}
		if (m == 4)
		{
			return 0;
		}
	}
	return 0;
}
