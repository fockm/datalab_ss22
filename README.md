# Data Analysis Lab

The purpose of this course is to provide an insight into the field of Data Analysis with large sets of experimental data. The students will learn to use and understand basic tools and methods which are used in real searches in gravitational wave and gamma-ray astronomy, such as those currently employed at AEI and LIGO.

# Table of Contents

[Resources](resources.md)

1. [Setting up](#lab-1)
    1. [Prerequisites](#prerequisites)
    2. [Task 1](#task-1)
    3. [Task 2](#task-2)
3. [Exercises](#exercises)
4. [Parallel Programming](#parallel-programming)


## Lab 1

### Prerequisites

#### Get git and a github account

* Install [git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).
* [Create a github account](https://github.com/join) (if you don't have one aready) and log into it. 

#### Generate a ssh key

> To make your life easier when you upload the solutions to the next exercises you should now generate on your machine a `ssh` key that will allow you to do operations on your repository without being asked for a username and password each time. 

*  Generate a ssh key using the terminal/command line (Try [this link](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) first and try to figure it out. Go through with step 7 of adding your ssh key to your github account as well! -- If all else fails [here](/instructions/ssh-key.md) is a more detailed guide)

#### Get the code

* Once you are logged in with your account, fork this repository by pressing the fork button on the upper right corner of this repository's page. 

![Screenshot 2021-10-14 at 11 47 03](https://user-images.githubusercontent.com/6952640/137299631-11d5982d-b60c-4484-943e-0e0b8f22ea26.png)

> Now you should have __your own repository__ in your namespace called datalab_ss22 `<username>/datalab_ss22`.

* You should also have a ssh key added to your account to continue - if not use the _'HTTPS'_ link for the repository - you will be prompted for a username and password everytime. Copy the git url of this repository by going to your github page, the repository and clicking on _Code_>_SSH_>_copy_:

![Screenshot 2021-10-14 at 11 48 48](https://user-images.githubusercontent.com/6952640/137303888-76e1d354-bb03-4cc3-8d99-662cd15bdbb3.png)

* Open a command line/terminal an clone your repository. The command should look something like:

```
git clone git@github.com:<username>/datalab_ss22.git
```

This will automatically create a new folder called `datalab_ss22` inside the folder where you ran the command and will give you an error if such a folder exists. If you want the folder to have another name run `git clone gitithub.com:<username>/datalab_ss22.git <new_folder_name>`, e.g: `git clone gitithub.com:<username>/datalab_ss22.git datalab`. If you want to move the entire folder after you have cloned it, everything will work fine as the git references are kept in hidden files inside the folder.

#### Your first commit

* Create a new file in `datalab/solutions/exercise_1.py` and push your changes to your repository. 

<details>
  <summary>Solution here</summary>

  Go to your `datalab` folder. Make a new folder called `solutins`:
  ```
  $ mkdir solutions
  ```
  Create a new file called `exercise_1.py` with any method.
  ```
  $ touch solutions/exercise_1.py
  ```
  Check the changes to your repository
  ```
  $ git status
  ```
  Commit the changes and then push them:
  ```
  $ git add . 
  $ git commit -m "Saving my changes."
  $ git log
  $ git push origin main
  ```
</details>

#### Rebase from upstream

To get new changes that are pushed to __this__ main repository the simplest way is to add an upstream and rebase your code. Before you rebase you should commit all your local changes that you want to keep. Try it yourself using this [link](https://medium.com/@topspinj/how-to-git-rebase-into-a-forked-repo-c9f05e821c8a)

<details>
  <summary>Solution here</summary>

  Go to your `datalab` folder.
  To see what repositorities you are tracking run `git remote -v` - The output will probably look like this
  ```
  $ git remote -v
  origin	git@github.com:<your_username>/datalab_ss22.git (fetch)
  origin	git@github.com:<your_username>/datalab_ss22.git (push)
  ```
  Because you did the fork from the interface you can also get the new changes from the interface. But the better way to it is to add a _'remote'_ pointing to the fork (Add a keyname for the main repository). The textbook name for a repo you forked from is __upstream__. 
  Add a remote named _upstream_ pointing to *this* repo using: `git remote add upstream git@github.com:alebot/datalab_ss22.git`. Now when you run `git remote -v`you should see something *like* this:
  ```
  $ git remote -v
  origin	git@github.com:<your_username>/datalab.git (fetch)
  origin	git@github.com:<your_username>/datalab.git (push)
  upstream	git@github.com:alebot/datalab.git (fetch)
  upstream	git@github.com:alebot/datalab.git (push)
  ```
  The best way to pull the new changes is using the `rebase` comamnd. This means that any commits you have made will be _'rebased'_ onto the new changes in the repository you have forked. (Make sure you have commited all your changes before proceeding.
  ```
  $ git status
  $ git add . 
  $ git commit -m "Saving my changes."
  $ git log
  $ git fetch upstream
  $ git rebase upstream/main
  $ git log
  ```
</details>


### Task 1 

The first task will be to compile the two `C` source files. Go to your `datalab/code` folder and simply try run in your command line:

```
./Makefile
```

I expect you might get some errors, missing libraries, missing executable. Try to solve them. 

You can either do this in your local environment or use [docker](https://docs.docker.com/get-docker/) and run a container with c++ for example [this one](https://hub.docker.com/_/gcc). 
  
Test this is working correctly by running:
```
generate_source --help
prober --help
```

you should get no errors and just a help message. 
  
### Task 2
  
* Prepare your python environment. To solve the following exercises we will need preferebly [python3](https://www.python.org) installed and at least a plotting library (such as [matplotlib](https://matplotlib.org), but probably numpy, pandas etc will be useful as well. If you are using [anaconda](https://www.anaconda.com/products/individual) or [miniconda](https://docs.conda.io/en/latest/miniconda.html) make a new python environment for the datalab. 
  
* Have an IDE prepared, wheather it is [Jupyter](https://jupyter.org), [PyCharm](https://www.jetbrains.com/pycharm/), [Notebook++](https://notepad-plus-plus.org/downloads/), etc - the most important thing is you can easily work with it. Try to write a script that prints "Hello World!" and run it. 
  
## Exercises
 
We will go through the Exercise sheets and solutions together.
The final assignment sheet and data will be in the [assignment folder](https://github.com/alebot/datalab_ss22/tree/main/assignment) in this repo. 
    
## Parallel Programming 
    
Resources:
* [Introduction to HPC](https://hpc.llnl.gov/documentation/tutorials/introduction-parallel-computing-tutorial##Whatis) (High Performance Computing)
* [Introduction to MPI and OpenMP](https://princetonuniversity.github.io/PUbootcamp/sessions/parallel-programming/Intro_PP_bootcamp_2018.pdf)
* [Programming with CUDA: parallel reduction in GPUs](https://developer.download.nvidia.com/assets/cuda/files/reduction.pdf)
* [Patterns for Parallel Programming](https://www.researchgate.net/publication/234826291_Patterns_for_Parallel_Programming)
* [Ian Foster Book: Designing and building parallel programms](https://www.mcs.anl.gov/~itf/dbpp/text/book.html)
* [Ian Foster Course](https://edoras.sdsu.edu/~mthomas/docs/foster/Foster_Designing_and_Building_Parallel_Programs.pdf)
* [Online Course](https://open.hpi.de/courses/parprog2014)
* [Python multiprocessing Tutorial](https://www.youtube.com/watch?v=fKl2JW_qrso&t=399s)
* [pymp library](https://github.com/classner/pymp)
* [Condor](http://www.umiacs.umd.edu/labs/parallel/condorintro.htm)
* [Coding Game: Have fun with MPI](https://www.codingame.com/playgrounds/47058/have-fun-with-mpi-in-c/lets-start-to-have-fun-with-mpi)
* [Video: Introduction to parallel programming with MPI](https://www.youtube.com/watch?v=36nCgG40DJo)
* [mpi4py](https://mpi4py.readthedocs.io/en/stable/intro.html)
