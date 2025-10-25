#ifndef TreeN_H
#define TreeN_H

#include<iostream>
#include<iomanip>
#include<string>

using namespace std;
class Task
         {
           public:
            string element;
            int duration;
            int completionTime;
            bool critical;
            enum class Status { Pending, Passed, Failed } status;

            Task() : element(""), duration(0), completionTime(0),
                        critical(0), status(Status::Pending) {};
            Task( string theString  , int theDuration ,
                  int theCompletion,
                   bool isCritical )
                 : element(theString), duration(theDuration),
                    completionTime(theCompletion),
                     critical(isCritical), status(Status::Pending) {};

          };

        class TreeNode
        {
            Task info;
            TreeNode * leftChild;
            TreeNode * rightSibling;

            TreeNode() : info(), leftChild(NULL), rightSibling(NULL) {};

            TreeNode( Task theTask, TreeNode *lt, TreeNode *rt )
              : info( theTask ), leftChild( lt ), rightSibling( rt ) { }

            friend class Tree;
            friend class TreeItr;
        };

class TreeItr
        {
          public:
            TreeItr( ) : current( NULL ) { }
            bool isPast( ) const
              { return current == NULL; }
            bool hasChild( ) const
              { return current->leftChild != NULL; }

            void advanceSibling( )
              { if( !isPast( ) ) current = current->rightSibling; }
            void advanceChild( )
              { if( !isPast( ) ) current = current->leftChild; }

            Task getInfo()
              {  if( !isPast( ) ) return current->info; return Task(); }

            void changeInfo( Task  x)
        	{
           	 current->info = x;
        	}

          private:
            TreeNode *current;

            TreeItr( TreeNode *theNode )
              : current( theNode ) { }

            friend class Tree;
        };

        class Tree
        {
          public:
            explicit Tree( );
            bool isEmpty( ) const;
            TreeItr createChild( TreeItr p );
            TreeItr TreeRoot( ) const;
            TreeItr setupTree(Tree & T);
            void preorderPrint(TreeItr p);

            TreeItr addTask(TreeItr parent, Task newTask);
                // Update each task's status based on how many days have passed
                void updateStatusByElapsedDays(int daysPassed);
      // Calculate OKR: percentage of tasks marked Passed (0.0-100.0)
      double calculateOKRPercentage() const;
            bool deleteTask(TreeItr target);
            void printDiagram(TreeItr p, int depth = 0);

          private:
            TreeNode *root;

        };
#endif // TASK_H
