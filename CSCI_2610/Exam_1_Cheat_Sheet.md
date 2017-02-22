CSCI 2610 Exam 1 Cheat Sheet
============================

# Logic and Proofs

## Ways $p \rightarrow q$ is expressed
*   "if $p$, then $q$"
*   "$p$ implies $q$"
*   "if $p$, $q$"
*   "$p$ only if $q$"
*   "$p$ is sufficient for $q$"
*   "a sufficient condition for $q$ is $p$"
*   "$q$ if $p$"
*   "$q$ whenever $p$"
*   "$q$ when $p$"
*   "$q$ is necessary for $p$"
*   "a necessary condition for $p$ is $q$"
*   "$q$ follows from $p$"
*   "$q$ unless $\neg p$"

## Ways $p \leftrightarrow q$ is expressed
*   "$p$ is necessary and sufficient for $q$"
*   "if $p$ then $q$, and conversely"
*   "if $p$ then $q$, and if $q$ then $p$"
*   "$p$ iff $q$"

## Converse, Contrapositive, and Inverse
For $p \rightarrow q$ :
*   $q \rightarrow p$ is the **converse**.
*   $\neg p \rightarrow \neg q$ is the **inverse**.
*   $\neg q \rightarrow \neg p$ is the **converse**.

## Logical Equivalences

### Identity
$p \land \mathbf{T} \equiv p$  
$p \lor \mathbf{F} \equiv p$

### Domination
$p \lor \mathbf{T} \equiv \mathbf{T}$  
$p \land \mathbf{F} \equiv \mathbf{F}$

### Double Negation
$\neg(\neg p) \equiv p$

### Commutative
$p \lor q \equiv q \lor p$  
$p \land q \equiv q \land p$

### Associative
$(p \lor q) \lor r \equiv p \lor (q \lor r)$  
$(p \land q) \land r \equiv p \land (q \land r)$

### Distributive
$p \lor (q \land r) \equiv (p \lor q) \land (p \lor r)$  
$p \land (q \lor r) \equiv (p \land q) \lor (p \land r)$

### De Morgan's
$\neg(p \land q) \equiv \neg p \lor \neg q$  
$\neg(p \lor q) \equiv \neg p \land \neg q$

### Absorption
$p \lor (p \land q) \equiv p$  
$p \land (p \lor q) \equiv p$

### Negation
$p \lor \neg p \equiv \mathbf{T}$  
$p \land \neg p \equiv \mathbf{F}$

### With Conditionals
$p \rightarrow q \equiv \neg p \lor q$  
$p \rightarrow q \equiv \neg q \rightarrow \neg p$  
$p \lor q \equiv \neg p \rightarrow q$  
$p \land q \equiv \neg(p \rightarrow \neg q)$  
$\neg(p \rightarrow q) \equiv p \land \neg q$  
$(p \rightarrow q) \land (p \rightarrow r) \equiv p \rightarrow (q \land r)$  
$(p \rightarrow q) \lor (p \rightarrow r) \equiv p \rightarrow (q \lor r)$  
$(p \rightarrow r) \land (q \rightarrow r) \equiv (p \land q) \rightarrow r$  
$(p \rightarrow r) \lor (q \rightarrow r) \equiv (p \land q) \rightarrow r$

### With Biconditionals
$p \leftrightarrow q \equiv (p \rightarrow q) \land (q \rightarrow p)$  
$p \leftrightarrow q \equiv \neg p \leftrightarrow \neg q$  
$p \leftrightarrow q \equiv (p \land q) \lor (\neg p \land \neg q)$  
$\neg(p \leftrightarrow q) \equiv p \leftrightarrow \neg q$

## Propositional Satisfiability
*   **Satisfiable** if there is an assignment of truth values to variables to make propsition true
*   **Unsatisfiable** otherwise. Unsatisfiable if and only if its negation is a tautology.

## De Morgan's for Quantifiers
$\neg\exists\text{xP}(x) \equiv \forall x\neg P(x)$  
$\neg\forall\text{xP}(x) \equiv \exists x\neg P(x)$

## Rules of Inference

### Modus Ponens
${
\begin{aligned}
p\\p\rightarrow q\\
\therefore {\overline {q\quad \quad }}\\
\end{aligned}
}$

### Modus Tollens
${
\begin{aligned}
\neg q\\p\rightarrow q\\
\therefore {\overline {\neg p\quad \quad }}\\
\end{aligned}
}$

### Associative
${
\begin{aligned}
(p\lor q)\lor r\\
\therefore {\overline {p\lor (q\lor r)}}\\
\end{aligned}
}$

### Commutative
${
\begin{aligned}
p\land q\\
\therefore {\overline {q\land p}}\\
\end{aligned}
}$

### Law of Biconditional Propositions
${
\begin{aligned}
p\rightarrow q\\q\rightarrow p\\
\therefore {\overline {p\leftrightarrow q}}\\
\end{aligned}
}$

### Exportation
${
\begin{aligned}
(p\land q)\rightarrow r\\
\therefore {\overline {p\rightarrow (q\rightarrow r)}}\\
\end{aligned}
}$

### Transposition or Contraposition Law
${
\begin{aligned}
p\rightarrow q\\
\therefore {\overline {\neg q\rightarrow \neg p}}\\
\end{aligned}
}$

### Hypothetical Syllogism
${
\begin{aligned}
p\rightarrow q\\q\rightarrow r\\
\therefore {\overline {p\rightarrow r}}\\
\end{aligned}
}$

### Material Implication
${
\begin{aligned}
p\rightarrow q\\
\therefore {\overline {\neg p\lor q}}\\
\end{aligned}
}$

### Distributive
${
\begin{aligned}
(p\lor q)\land r\\
\therefore {\overline {(p\land r)\lor (q\land r)}}\\
\end{aligned}
}$

### Absorption
${
\begin{aligned}
p\rightarrow q\\
\therefore {\overline {p\rightarrow (p\land q)}}\\
\end{aligned}
}$

### Disjunctive Syllogism
${
\begin{aligned}
p\lor q\\
\neg p\\
\therefore {\overline {q\quad \quad }}\\
\end{aligned}
}$

### Addition
${
\begin{aligned}
p\\
\therefore {\overline {p\lor q}}\\
\end{aligned}
}$

### Simplification
${
\begin{aligned}
p\land q\\
\therefore {\overline {p \quad \quad }}\\
\end{aligned}
}$

### Conjunction
${
\begin{aligned}
p\\q\\
\therefore {\overline {p\land q}}\\
\end{aligned}
}$

### Double Negation
${
\begin{aligned}
p\\
\therefore {\overline {\neg \neg p}}\\
\end{aligned}
}$

### Disjunctive Simplification
${
\begin{aligned}
p\lor p\\
\therefore {\overline {p \quad \quad }}\\
\end{aligned}
}$

### Resolution
${
\begin{aligned}
p\lor q\\
\neg p\lor r\\
\therefore {\overline {q \lor r}}\\
\end{aligned}
}$

## Quantifier Inferences

### Universal Instantiation (UI)
${
\begin{aligned}
\forall x P(x)\\
\therefore {\overline {P(c) \quad}}
\end{aligned}
}$

### Universal Generalization (UG)
${
\begin{aligned}
P(c) \text{ for an arbitrary }c\\
\therefore {\overline{\forall x P(x) \quad\quad\quad\quad\quad\quad\quad}}
\end{aligned}
}$

### Existential Instantiation (EI)
${
\begin{aligned}
\exists x P(x)\\
\therefore {\overline{P(c) \text{ for some element } c}}
\end{aligned}
}$

### Existential Generalization (EG)
${
\begin{aligned}
P(c) \text{ for some element } c\\
\therefore {\overline{\exists x P(x) \quad\quad\quad\quad\quad\quad\quad}}
\end{aligned}
}$

## Types of Proofs
For $p \rightarrow q$ :

### Direct
Assume $p$ is true, then show $q$ must also be true.

### Contraposition
Assume $\neg q$ is true, then show $\neg p$ must also be true.

### Contradiction
Show that $\neg p \rightarrow q$ is invalid
(i.e. assume $\neg p$ and derive a contradiction such as $r \land \neg r$).

### Case / WLOG
For statements in the form 
  $$(p_1 \lor p_2 \lor \dots \lor p_n ) \rightarrow q$$
use tautology
  $$
  [(p_1 \lor p_2 \lor \dots \lor p_n ) \rightarrow q] 
  \leftrightarrow 
  [(p_1 \rightarrow q) \land (p_2 \rightarrow q) \land \dots \land (p_n \rightarrow q)]
  $$
i.e. show that for all possible cases of the proposition, it is true.

Note: cases must be such that they apply for all cases, 
i.e. *without loss of generality* (WLOG).

#### Universally Quantified Assertions
For form $\forall x P(x)$, assume $x$ is arbitrary member of domain and show that
$P(x)$ must be true. Using `UG` it follows that $\forall P(x)$.

#### Existence Proofs
For form $\exists x P(x)$, find an explicit value $c$, for which $P(c)$ is true.
Using `EG` it follows that $\exists x P(x)$.

#### Nonconstructive Existence Proofs
Assume no $c$ exists that makes $P(c)$ true and derive a contradiction.

### Counterexample
Basically find an example that makes a proposition false.

### Uniqueness Proofs
There exists a unique $x$ such that $P(x)$ - $\exists !x P(x)$.

Two parts:
*   *Existence*: show that the $\exists$ part is true.
*   *Uniqueness*: show that if $y\neq x$, then $y$ does not have the property.

# Boolean Algebra
*   CNF
*   DNF
