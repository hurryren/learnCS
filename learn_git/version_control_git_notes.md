[tutorial address](https://missing.csail.mit.edu/2020/version-control/)





## Version Control(Git)



Version control systems(VCSs) are tools used to track changes code(or other collections of files and folders). As the name implies, these tools help maintain a history of changes; furthermore, they facilitate collaboration. VCSs track changes to a folder and its contents in a series of snapshots, where each snapshot encapsulates the entire state of files/folders within a top-level directory. VCSs also maintain metadata like who created each snapshot, messages associated with each snapshot, and so on.

Why is version control useful? Even when you are working by yourself, it can let you look at old snapshots of a project, keep a lot of why certain changes were made, work on parallel branches of development, and much more. When working with others, it’s an invaluable tool for seeing what other people have changes, as well as resolving conflicts in concurrent development.

Modern VCSs also let you easily(and often automaticlly ) answer qustions like:

-   who wrote this module?
-   When was this particular line of this particular file edited? By whom> Why wa is edited?
-   Over the last 1000 revisions, when/why did a particular unit test stop working?



### Git’s data model

#### Snapshots

Git models the history of a collection of files and folders within some top-level directory as a series of snapshots. In Git terminology, a file is called a “blob”, and it  is just a bunch of bytes. A directories can contain other directories). A snapshot is the top-level tree that is being tracked. For example, we might have a tree as follows:

```txt
<root> (tree)
|
+- foo (tree)
|	|
|	+ bar.txt (blob, contents = "hello world")
|
+-	baz.txt (blob, contents = "git is wonderful")
```

The top-level tree contains two elements, a tree “foo”(that itself contains one element, a blob “bar.txt”), and a blob “bax.txt”.



#### Modeling history: relating snapshots

How should a version control system relate snapshots? One simple model would be to have a lineat history. A history would be a list of snapshots in time-order. For many reasons, Git does not use a simple module like this.

In Git, a history is a directory acyclic graph(DAG, 有向无环图) of snapshots. That may sound like fancy math word, but do not be itimidated. All this means is that each snapshot in Git refers to a set of “parents”, the snapshots that preceded it. It’s a set of parents rather than a single parent(as would be the case in a linear history) because a snapshot might descend from multiple parents, for example, due to combining (merging) two parallel branches of development.

Git calls these snapshots “commit”s, Visualizing a commit history might look sonething like this”

```txt
O <-- O <-- O <-- O
			^
			 \
			   --- O <-- O
```

In the ACSLL art above, the O s correspond to individual commits (snapshots).

The arrows point to the parent of each commit (it’s a comes before relation, not comes after). After the third, the history branches into tow separate branches. This might correspond to, for example, tow separate features being developed in paralll, indpndntly rom ach othr. In the future, these branches may be merges to create a new snapshot that incorporates both of the featheres, producing a new history that looks like this, with the newly created merge commit shown in bold:

```txt
O <-- O <-- O <-- O <---- O
			^			 /
			 \			 v
			   --- O <-- O
```

Commits in Git are immutable. This doesn;t mean that mistakes can’t be corrected, however; It’s just that edits to the commit history are actually creating entirely new commits, and references are updated to point to th nw ons.



#### Data model, as pseudocode

It may be instructive to see Git’s data module written down in pseudocode:

```txt
// a file is a bunch of byts
type blob =  array<byte>

// a directory contains named files and directories
type tree = map<string, tree | blob>

// a commit has parents, metadata, and the top-level tree
type commit = struct {
	parents: array<commit>
	author: string
	message: string
	snapshot: tree
}
```



#### Objects and contents-addressing

An “object” is a blob, tree, or commit:

```txt
type object = blob | tree | commit
```

In Git data store, all objects are content-addressed by their SHA-1 hash.

```txt
objects = map<string, object>

def store(object):
	id = sha1(object)
	objects[id] = object

def load(id):
	return objects[id]
```

Blobs, trees, and commits are unifies in this way: they are all objects. When they reference other objects, they do not actually contrain them in their on-disk representaion, but have a reference to them by their hash.

For example, the tree for the example directory structure above(visualized using git cat-file -p 698281bc680d1995c5f4caaf3359721a5a58d48d), looks like this:

```txt
100644 blob 4448adbf7ecd394f42ae135bbeed9676e894af85    baz.txt
040000 tree c68d233a33c5c06e0340e4c224f0afca87c8ce87    foo
```

The tree itself contains pointers to its contents, baz.txt(a blob)  and foo(tree). If we loop at the contents addressed by the hash corresponding to baz.txt with git-cat file -p 4448adbf7ecd394f42ae135bbeed9676e894af85, we get following:

```txt
git is wonderful
```



#### References

Now, all snapshots can be identified by their SHA-1 hashes. That’s inconvenient, because humans are not good at remembering string of 40 hexadecimal characters.

Git’s solution to this problem is human-readable names for SHA-1 hashes, called “references”. References are pointers to commits. Unlike objects, which are immutable, references are mutable(can be updated to point to a new commit). For example, the **master** reference usually points to the latest commit in the main branch of development.

```txt
references = map<string, string>

def update_reference(name, id):
	references[name] = id
	
def read_reference(name):
	return references[name]

def load_reference(name_or_id):
	if name_or_id in in references:
		return load(references[name_or_id])
	else:
		return load(name_or_id)
```

With this, Git can use human-readable names like “master” to refer to a particular snapshot in the history, instead of a long hexadecimal string.

One detail is that we often want a notion of “where we currently are” in the history, so that when we take a new snapshot, we know what it is relative to(how we set the parents field of the commit). In Git, that where we currently are is a special referece called **HEAD**.

#### Repositories

Finally, we can define what (roughly) is a Git repository: it is the data **objects** and **references**.

On disk, all Git stores are objects and references: that’s all there is to Git’s data model. All git commands map to some manipulation of the commit DAG by adding objects and adding/updating references.

Whenever you are typing in any command, think, about what manipulation the command is making to the underlying graph data structure. Conversely, if you are trying to make a particular king of change to the commit DAG, e.g. “discard uncommitted changes and make the master ref point to commit 5d83f9e”, there is probable a command to do it(e.g. in this case, git checkout master; git reset --hard 5d83f9e).



### Git command-line interface

#### Basics

-   *git help <commond>*:  get help for a git command
-   *git init*: creates a new git repo, with data stored in the *.git* firectory
-   *git status*: tells you what is going on
-   *git add <filename>*: adds files to staging area
-   *git commit*: creates a new commit
-   *git log*: shows a flattened log history
-   *git log --all --graph --decorate*: visualizes history as a DAG
-   *git diff <filename>*: show changes you made relative to the staging area
-   *git diff <revision> <filename>* : shows differences in a file between snapshots
-   *git checkout <revision>*: updates HEAD and current branch



#### Branching and merging

-   *git branch* : show branches
-   *git branch <name>*: creates a branch
-   *git checkout -b <name>*: creates a branch and switches to is. same as git branch <name>; git checkout <name>.
-   *git merge <revision>*: merges into current branch
-   *git mergetool*: use a fancy tool to help resolve merge confilicts
-   *git rebase*: rebase a set of patches onto a new base



#### Remotes

-   *git remote*: list remotes
-   *git remote add <name>  <url>*: add a remote
-   *git push <remote> <local branch>:<remote branch>*: senf objects to remote, and update remote reference
-   *git branch --set-upstream-to=<remote>/<remote a branch>*: set up correspondence between local and remote branch
-   *git fetch*: retrieve objects/references from a remote
-   *git pull*: same as git fetch; git merge
-   *git clone*: download repository from remote



#### Undo

-   *git commit --amend*: edit a commit’s contents/message
-   *git reset HEAD <file>*: unstage a file
-   *git checkout -- <file>*: discard changes
-   



















































