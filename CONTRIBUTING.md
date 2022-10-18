# Contribution guide
If you want to work on this project, great! Here's info on how to do it.

## Table of contents
- [Getting started](#getting-started)
- [Building](#building)
- [Contributing changes to upstream](#contributing-changes-to-upstream)
- [Some tips](#some-tips)

## Getting started
First ask the team members if there's any work that you could do. When you figure out what you want to do
set up your project. Start by forking the project on GitHub and then cloning the fork onto your own machine
> git clone *link_to_your_fork*

Add a new remote. (We will call it upstream in the rest of this guide)
> git remote add upstream git@github.com:stolo93/IFJ_2022.git

The above command uses ssh authentication. For https authentication use
> git remote add upstream https://github.com/stolo93/IFJ_2022.git

If you're using a git front-end ask [DuckDuckGo](https://duck.com) how to add a new remote to your front-end.

Don't forget to fetch the remote
> git fetch upstream

And you're done with the setup, you're ready to create a new branch in your fork and start with your work.
When you're ready to have your work added to the project read [Contributing changes to upstream](#contributing-changes-to-upstream).

## Building
To build this project just run ```make```. We have no external dependencies.

## Contributing changes to upstream
Before pushing your branch upstream it needs to be rebased onto the newest upstream commits.
First sync your local main with the upstream main
> git checkout main
> 
> git pull upstream main

Now as your repository is in sync with upstream rebase your branch onto main.
Use the interactive mode to clean up your branch and solve any merge conflicts.
> git checkout *feature_branch*
> 
> git rebase --interactive main

Now, as the rebase is finished, it is time to push the branch into origin
> git push origin *feature_branch*

Finally go to GitHub and create a pull request from your branch onto tha main upstream branch.
Wait for the reviewers and fix any mistakes

When the Pull Request gets approved it's time to sync the local main with the upstream main
> git checkout main
> 
> git pull upstream main

If any issues pop up the --force flag should help

###### The entire timeline
1. `Pull latest commits into your main`
2. `Rebase your branch onto your main`
3. `Push your branch into your fork`
4. `Create a pull request into upstream main`

## Some tips
###### Merge tool
If you don't have a mergetool setup for your git workflow one of our team members recommends Meld
> git config --global mergetool.meld

###### DEFINE_VECTOR_PROTOTYPES complains
If you use DEFINE_VECTOR_PROTOTYPES for a new type, you need typedef a type_ptr for your type.
I.E  `DEFINE_VECTOR_PROTOTYPES(int, int)` requires `typedef int* int_ptr`

###### Threats of violence
If you fuck anything up we reserve the right to threaten you with violence!!!
