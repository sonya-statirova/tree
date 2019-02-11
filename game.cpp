#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMINS_FILE "new_answers.txt"

class Node
{
  private:
    char* data_;
    Node* parent_;
    Node* left_;
    Node* right_;

  public:
    Node();
    Node(char* data);
    Node(Node* node);
    ~Node();
    void Print_node();
    friend Node* Scan_tree(FILE* fileptr, Node* parent = NULL);
		friend Node* Scan_node(FILE* fileptr, Node* parent);
		Node* Ask_or_Answer();
		Node* Process_questions();
		Node* Process_answers();
		Node* Find_node_by_data(char* data);
		friend void Put_new_answer_into_tree(FILE* fileptr /*r+*/);
		void Choose_who_is_left_who_is_righ(Node* prog_answ, Node* your_answ);
    void Preorder_traverse_Print_tree(FILE* fileptr, size_t count = 0);
    Node* Preorder_traverse_Copy_tree(Node* arg = NULL);
    void Postorder_traverse_Delete_tree();
};

//---------------------------------------------

int Check_file(FILE* fileptr, size_t count = 0);
int Process_new_answer(FILE* fileptr, char* prog_answ);
void Game(FILE* fileptr);
int Make_new_game_nodes(FILE* fileptr, Node* root, \
Node** prog_answ = NULL, Node** your_answ = NULL, Node** question = NULL);

//=============================================

Node::Node():
  data_ (NULL),
  parent_(NULL),
  left_ (NULL),
  right_ (NULL)
{}

//---------------------------------------------

Node::Node(char* data):
  data_ ((char*) calloc(strlen(data) + 1, sizeof(char))),
  parent_ (NULL),
  left_ (NULL),
  right_ (NULL)
  {
    strcpy(data_, data);
  }

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
}

//=============================================

int Check_file(FILE* fileptr, size_t count)
{
	char curchar = '\0';
	for (size_t i = 0; i < count; i++)
	{
		curchar = getc(fileptr);
		if (curchar == '}' && i == count - 1)
		{
			fseek(fileptr, -count, SEEK_CUR);
			return 1;
		}
		else if (curchar != '\t')
			return -1;
	}

  if (getc(fileptr) == '{')
  {
    fscanf(fileptr, "%*[^\n]");
		getc(fileptr);
    int left = Check_file(fileptr, count + 1);
    if (left == 0)
		{
      if (Check_file(fileptr, count + 1) != 0)
				return -1;
		}
		else if (left == -1)
			return -1;

		for (size_t i = 0; i < count; i++)
		{
    	if (getc(fileptr) != '\t')
				return -1;
		}
    if (getc(fileptr) != '}' || getc(fileptr) != '\n')
			return -1;
  }

  return 0;
}

//---------------------------------------------

void Node::Print_node()
{
  printf("%s", data_);
}

/*void Node::Print_node()
{
	printf("this: %lu;  ", this);
  printf("data: %s;  ", data_);
  printf("parent: %lu;  ", parent_);
  printf("left: %lu;  ", left_);
  printf("right: %lu\n\n", right_);
}*/

//---------------------------------------------

Node* Scan_tree(FILE* fileptr, Node* parent)
{
  fscanf(fileptr, "%*[\t]");
  if (getc(fileptr) == '{')
  {
    Node* root = Scan_node(fileptr, parent);
    root->left_ = Scan_tree(fileptr, root);
    if (root->left_ != NULL)
      root->right_ = Scan_tree(fileptr, root);
    //root->Print_node();
    fscanf(fileptr, "%*[\t]");
    getc(fileptr);
    getc(fileptr);

    return root;
  }
  else
    fseek(fileptr, -1, SEEK_CUR);

  return NULL;
}

//---------------------------------------------

Node* Scan_node(FILE* fileptr, Node* parent)
{
  size_t len = 0;
  size_t begining = ftell(fileptr);
  char check = getc(fileptr);

  while (check != '\n' && check != EOF)
  {
    len++;
    check = getc(fileptr);
  }

  fseek(fileptr, begining, SEEK_SET);
  Node* node = new Node();
  node->data_ = (char*) calloc(len + 1, sizeof(char));
  fread(node->data_, sizeof(char), len, fileptr);

	getc(fileptr);

  node->parent_ = parent;

  return node;
}

//---------------------------------------------

void Game(FILE* fileptr)
{
  Node* root = Scan_tree(fileptr);

  Node* node = root;
  while (node != NULL)
    node = node->Ask_or_Answer();

	printf("\nTHE END\n");
	printf(" =^.^=\n\n");

  root->Postorder_traverse_Delete_tree();
}

//---------------------------------------------

Node* Node::Ask_or_Answer()
{
  if (left_ != NULL && right_ != NULL)
    return this->Process_questions();
  else
		return this->Process_answers();
}

//---------------------------------------------

Node* Node::Process_questions()
{
	char answ[5] = {};

	while (true)
	{
		printf("%s\n", data_);
		scanf("%4s", answ);

		if (strcmp("yes", answ) == 0)
			return left_;
		else if (strcmp("no", answ) == 0)
			return right_;
		else
		{
			scanf("%*s");
			printf("Incorrect format of answer. Try again.\n");
		}
	}
}

//---------------------------------------------

Node* Node::Process_answers()
{
	char answ[5] = {};

	printf("Have you picked: %s?\n", data_);
	while (true)
	{
		scanf("%4s", answ);

		if (strcmp("no", answ) == 0)
		{
			Process_new_answer(fopen(ADMINS_FILE, "a"), data_);
			return NULL;
		}
		else if (strcmp("yes", answ) != 0)
		{
			scanf("%*s");
			printf("Incorrect format of answer. Try again.\n");
		}
		else
			return NULL;
	}
}

//---------------------------------------------

int Process_new_answer(FILE* admins_fileptr, char* prog_answ)
{
	printf("Please, enter your answer:\n");
	char* your_answ = (char*) calloc(61, sizeof(char));
	scanf("%60s", your_answ);

	fprintf(admins_fileptr, "program answer: %s\n", prog_answ);
	fprintf(admins_fileptr, "your answer: %s\n", your_answ);
	fprintf(admins_fileptr, "___________________________\n");

	printf("Admin\'ll check your answer later and put it into game.\n");

	fclose(admins_fileptr);

	return 0;
}

//---------------------------------------------

void Put_new_answer_into_tree(FILE* fileptr /*r+*/)
{
	Node* prog_answ = NULL;
	Node* your_answ = NULL;
	Node* question = NULL;

	Node* root = Scan_tree(fileptr);
	FILE* admins_fileptr = fopen(ADMINS_FILE, "r");

	while (Make_new_game_nodes(admins_fileptr, root, &prog_answ, &your_answ, &question) != 1)
	{
		if (prog_answ->parent_->left_ == prog_answ)
			prog_answ->parent_->left_ = question;
		else
			prog_answ->parent_->right_ = question;

		question->parent_ = prog_answ->parent_;
		prog_answ->parent_ = question;
		your_answ->parent_ = question;

		question->Choose_who_is_left_who_is_righ(prog_answ, your_answ);
	}

	fclose(admins_fileptr);

	fseek(fileptr, 0, SEEK_SET);
	root->Preorder_traverse_Print_tree(fileptr);
	root->Postorder_traverse_Delete_tree();
}

//---------------------------------------------

int Make_new_game_nodes(FILE* admins_fileptr, Node* root, \
Node** prog_answ, Node** your_answ, Node** question)
{
	fscanf(admins_fileptr, "%*[^:EOF]");
	if (getc(admins_fileptr) == EOF)
		return 1;
	getc(admins_fileptr);
	char* prog_data = (char*) calloc(61, sizeof(char));
	fscanf(admins_fileptr, "%60[^\n]", prog_data);

	fscanf(admins_fileptr, "%*[^:]");
	getc(admins_fileptr);
	getc(admins_fileptr);
	char* your_data = (char*) calloc(61, sizeof(char));
	fscanf(admins_fileptr, "%60[^\n]", your_data);

	*prog_answ = root->Find_node_by_data(prog_data);
	*your_answ = new Node(your_data);

	printf("Enter new question for difference between %s and %s:\n", prog_data, your_data);
	char* question_data = (char*) calloc(101, sizeof(char));
	scanf("%100[^\n]", question_data);
	*question = new Node(question_data);

	free(prog_data);
	free(your_data);
	free(question_data);

	return 0;
}

//---------------------------------------------

Node* Node::Find_node_by_data(char* data)
{
	if (strcmp(data_, data) == 0)
		return this;

	Node* check = NULL;

	if (left_ != NULL)
	{
		check = left_->Find_node_by_data(data);
		if (check != NULL)
			return check;
	}
	if (right_ != NULL)
		return right_->Find_node_by_data(data);

	return NULL;
}

//---------------------------------------------

void Node::Choose_who_is_left_who_is_righ(Node* prog_answ, Node* your_answ)
{
	printf("%s for %s should be \"yes\" or \"no\"?\n", data_, prog_answ->data_);
	char answ[5] = {};

	while (true)
  {
    scanf("%4s", answ);

    if (strcmp("yes", answ) == 0)
    {
			left_ = prog_answ;
			right_ = your_answ;
			break;
		}
    else if (strcmp("no", answ) == 0)
    {
			left_ = your_answ;
			right_ = prog_answ;
			break;
		}
    else
    {
      scanf("%*s");
      printf("Incorrect format of answer. Try again.\n");
    }
  }
}

//---------------------------------------------

void Node::Preorder_traverse_Print_tree(FILE* fileptr, size_t count)
{
  size_t i = 0;
  for (i = 0; i < count; i++)
    fprintf(fileptr, "\t");

  fprintf(fileptr, "{%s\n", data_);
  if (left_ != NULL)
    left_->Preorder_traverse_Print_tree(fileptr, count + 1);
  if (right_ != NULL)
    right_->Preorder_traverse_Print_tree(fileptr, count + 1);

  for (i = 0; i < count; i++)
    fprintf(fileptr, "\t");

  fprintf(fileptr, "}\n");
}

//---------------------------------------------

Node* Node::Preorder_traverse_Copy_tree(Node* arg)
{
  Node* new_root = new Node(this);

  new_root->parent_ = arg;

  if (left_ != NULL)
    new_root->left_ = left_->Preorder_traverse_Copy_tree(new_root);
  if (right_ != NULL)
    new_root->right_ = right_->Preorder_traverse_Copy_tree(new_root);

  return new_root;
}

//---------------------------------------------

void Node::Postorder_traverse_Delete_tree()
{
  if (this->left_ != NULL)
    this->left_->Postorder_traverse_Delete_tree();
  if (this->right_ != NULL)
    this->right_->Postorder_traverse_Delete_tree();

  delete this;
}
