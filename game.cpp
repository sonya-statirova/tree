#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Node
{
  private:
    char* data_;
    Node* parent_;
    Node* left_;
    Node* right_;

  public:
    Node();
    Node(Node* node);
    ~Node();
    friend Node* Scan_node(FILE* fileptr, Node* parent);
    void Print_node();
    friend Node* Scan_tree(FILE* fileptr, Node* parent = NULL);
    Node* Ask_or_Answer();
    friend void Game(FILE* fileptr);
    void Preorder_traverse_Print_tree(size_t count = 0); //root->Preorder_traverse_Print_tree();
    Node* Preorder_traverse_Copy_tree(Node* arg = NULL); //root->Preorder_traverse_Copy_tree();
    void Postorder_traverse_Delete_tree(); //root->Postorder_traverse_Delete_tree();
};

//=============================================

Node::Node():
  data_ (NULL),
  left_ (NULL),
  right_ (NULL)
{}

//---------------------------------------------

Node::Node(Node* node):
  data_ ((char*) calloc(strlen(node->data_) + 1, sizeof(char))),
  parent_ (NULL),
  left_ (NULL),
  right_ (NULL)
{
  strcpy(data_, node->data_);
}

//---------------------------------------------

Node::~Node()
{
  if (data_ != NULL)
    free(data_);
  if (parent_ != NULL)
    delete parent_;
  if (left_ != NULL)
    delete left_;
  if (right_ != NULL)
    delete right_;
}

//=============================================

Node* Scan_node(FILE* fileptr, Node* parent)
{
  size_t len = 0;
  size_t beginning = ftell(fileptr);

  while (getc(fileptr) != '\n' || getc(fileptr) != EOF)
    len++;

  fseek(fileptr, beginning, SEEK_SET);
  Node* node = new Node();
  node->data_ = (char*) calloc(len + 1, sizeof(char));
  fread(node->data_, sizeof(char), len, fileptr);

  node->parent_ = parent;

  return node;
}

//---------------------------------------------

void Node::Print_node()
{
  printf("%s", data_);
}

//---------------------------------------------

Node* Scan_tree(FILE* fileptr, Node* parent)
{
  fscanf(fileptr, "%*[\t]");

  if (getc(fileptr) !='{')
    return NULL;

  Node* root = Scan_node(fileptr, parent);

  root->left_ = Scan_tree(fileptr, root);
  root->right_ = Scan_tree(fileptr, root);

  if (getc(fileptr) != '}')
    return NULL;

  return root;
}

//---------------------------------------------

Node* Node::Ask_or_Answer()
{
  char answ[10];

  if (left_ != NULL && right_ != NULL)
  {
    while (true)
    {
      printf("%s\n", data_);
      scanf("%s", answ);

      if (strcmp("yes", answ) == 0)
        return left_;
      else if (strcmp("no", answ) == 0)
        return right_;
      else
        printf("Incorrect format of answer. Try again.\n");
    }

  }
  else
  {
    printf("Have you picked: %s?", data_);
    return NULL;
  }
}

//---------------------------------------------

void Game(FILE* fileptr)
{
  Node* root = Scan_tree(fileptr);

  Node* node = root;
  while (node != NULL)
    node = node->Ask_or_Answer();

  root->Postorder_traverse_Delete_tree();
}

//---------------------------------------------

void Node::Preorder_traverse_Print_tree(size_t count)
{
  size_t i = 0;
  for (i = 0; i <= count; i++)
    printf("\t");

  printf("{%s\n", data_);
  left_->Preorder_traverse_Print_tree(count + 1);
  right_->Preorder_traverse_Print_tree(count + 1);

  for (i = 0; i <= count; i++)
    printf("\t");

  printf("}");
}

//---------------------------------------------

Node* Node::Preorder_traverse_Copy_tree(Node* arg)
{
  Node* new_root = new Node(this);

  new_root->parent_ = arg;

  new_root->left_ = left_->Preorder_traverse_Copy_tree(new_root);
  new_root->right_ = right_->Preorder_traverse_Copy_tree(new_root);

  return new_root;
}

//---------------------------------------------

void Node::Postorder_traverse_Delete_tree()
{
  left_->Postorder_traverse_Delete_tree();
  right_->Postorder_traverse_Delete_tree();
  delete this;
}

//=============================================

int main()
{

  FILE* fp = fopen("teachers.txt", "r");
  Game(fp);

  return 0;
}
