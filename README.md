# NetworkFlow
In this project, you are asked to find an efficient algorithm to assign publications for a group of academicians (reviewers) to review a paper.

For the acceptance of a paper to be published on any platform, e.g. journal, book, or conference, they should be reviewed by different academicians. However, each platform can have its own conditions. For example, if a writer wants to be published his/her paper on a journal, the paper should be reviewed by 2 reviewers. Also, in some publication platforms, only specific kind of titled academician’s reviews are accepted.

In this project, suppose you have m reviewers and n publications. Each reviewer is labeled from 1 to m and similarly, each publication is marked from 1 to n.
Also, there are 3 types of publication platforms, which are journal, conference and workshop and publications are grouped by requested publication platform.

Each publication i must be reviewed exactly certain number of reviewers based on the conditions of requested publication platform:

    * A paper must be reviewed by 2 reviewers to be published in a journal.
    * A paper must be reviewed by 3 reviewers to be published in conference proceeding.
    * A paper must be reviewed by 2 reviewers to be published in workshop proceeding.
    
Moreover, reviewers are grouped by their titles because some publications must be evaluated by a specific titled academician. In this project, 3 types of titles exist: Professor, Assistant Professor and Research Assistant.

These titles also identify for which publication platform they can make review. The following table shows this information:

![alt text](https://i.imgur.com/d8gRKrM.jpg)

Each reviewer j can study on just one publication at a time.

You have the information for each reviewer (reviewer.txt), describing the how much time in total they can spend for reviewing. Also, publication.txt gives the list of all publications. First line of these files indicate the number of reviewers and publications, respectively.

An output file (output.txt) should be generated at the end of the execution that contains the assignment for each publication. If a valid assignment does not exist, an error message (“Publication #i cannot be assigned.”) should be written to the file.

Your program should compile and run using the following commands:

    g++ networkflow.cpp –o project3
    ./project3 reviewer.txt publication.txt
