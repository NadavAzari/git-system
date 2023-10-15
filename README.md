# My own git system

## Important to know
## (I DIDNT FINISH CODING, THE PROJECT STILL GOT BUGS BUT ITS ALMOST FINISHED)
Before trying and using my own basic implementation version of git,
Its good to know, If your project is big, more than one participant, uses remote git server, merging between branches?
then **dont use snit**.
Also, snit cannot take control over all the edge cases, snit implementation does not contains optimizations like git.
So when can you try snit?

+ Want two seperates worktrees in the same dir?
  + Snit supports branches🫠
+ Want to manage versions and be able to downgarde your worktree to lower versions?
  + Snit supports commits💀

## Usage

**Creating an empty repo:**
```bash
snit init
```
**Adding files/dirs for staging:**
```bash
snit add file1 file2 dir1 # and so on
```

**Removing files/dirs**
```bash
snit rm file1 file2 dir1 # and so on
```

**Commiting new worktree**
```bash
snit commit "My commit message"
```

**Commits history**
```bash
snit log # No joke this log is nicer than git
```

**Branches list / delete branch**
```bash
snit branch # or use snit branch -d branch_name to delete
```

**Navigate between branches**
```bash
snit checkout branch_name # -b to create new branch
```


