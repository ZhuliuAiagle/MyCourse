# <center>Assignment I</center>

<center>SHEN ZIJIN(沈子衿) 3160104734</center>

### 1. Machine Learning Problems

#### (a)

1) BF 	2) C 	3) C 	4) BG 	5) AE 	6) AD 	7) BF 	8) AE 	9) BF

#### (b)

$Answer$: $FALSE$.   On the one hand, the data at hand should be divided into training sets and test sets, not all for training. On the other hand, we need to properly control the number of training sets and test sets to maximize the performance of the test set. Too many training sets can lead to overfitting.



### 2. Bayes Decision Rule

#### (a)

(1)  Since each box is selected as an independent event:
$$
P(B_1 = 1) = \frac{1}{3}
$$
(2) Since there is only one prize and that prize has been found in B1:
$$
P(B_2 = 0 \mid B_1 = 1) = 1
$$
(3) According to Bayes Theorem：
$$
P(B_1 = 1 | B_2 = 0) = \frac{P(B_2 = 0 | B_1 = 1)P(B_1 = 1)}{P(B_2 = 0)} = \frac {1×\frac{1}{3}}{\frac{1}{1}} = \frac{1}{3}
$$
(4) Since
$$
P(B_3=1\mid B_2=0)==\frac{2}{3}
$$
We **should** change our decision.



#### (b)

(i) The distribution graph are as follows:

![](.\img\2b_1.png)

And the test error, according to the output, is $64$.

(ii) The distribution graph are as follows:

![](img\2b_2.png)

And the test error, according to the output, is $47$.

(iii) Minimum risk, according to the output, is $0.242667$, (**TAKE ALL DATA AND P(X) INTO CONSIDERATION**).

(*Tips.*) The screenshot of the final output of MATLAB script(run.m):

![](img\2b_result.png)



### 3. Gaussian Discriminant Analysis and MLE 

#### (a)

According to Bayes' theorem:
$$
\begin{align}
P(y=1\mid \mathtt{x};\phi,\mu_0, \mu_1, \Sigma_0, \Sigma_1)&=
\frac{P(\mathtt{x};\phi,\mu_0,\mu_1,\Sigma_0, \Sigma_1\mid y = 1)P(y = 1)}{P(\mathtt{x};\phi,\mu_0,\mu_1,\Sigma_0, \Sigma_1)}
\end{align}
$$


According to the definition of conditional probability:
$$
P(\mathtt{x}) = P(\mathtt{x}\mid y = 0)P(y = 0) +  P(\mathtt{x}\mid y = 1)P(y = 1)
$$
According to the conditions given, we can also know that：
$$
\Sigma_0=\Sigma_1=\Sigma=\begin{pmatrix} 1 & 0\\ 0 & 1 \end{pmatrix}=I, \phi=\frac{1}{2}, \mu_0=\left(0, 0\right), \mu_1=\left(1, 1\right)^T
$$
Combine the above expressions to get:
$$
\begin{align}
P(y=1\mid \mathtt{x};\phi,\mu_0, \mu_1, \Sigma_0, \Sigma_1)&=
\frac{P(\mathtt{x};\phi,\mu_0,\mu_1,\Sigma_0, \Sigma_1\mid y = 1)P(y = 1)}{P(\mathtt{x};\phi,\mu_0,\mu_1,\Sigma_0, \Sigma_1)}
\\&=
\frac{P(\mathtt{x}\mid y = 1)P(y = 1)}{P(\mathtt{x}\mid y = 0)P(y = 0) +  P(\mathtt{x}\mid y = 1)P(y = 1)}
\\&=
\frac{\frac{1}{4\pi}e^{-\frac{1}{2}[(x_1-1)^2+(x_2-1)^2]}}{\frac{1}{4\pi}e^{-\frac{1}{2}(x_1^2+x_2^2)}+\frac{1}{4\pi}e^{-\frac{1}{2}[(x_1-1)^2+(x_2-1)^2]}}
\\&=
\frac{e^{-\frac{1}{2}[(x_1-1)^2+(x_2-1)^2]}}{e^{-\frac{1}{2}(x_1^2+x_2^2)}+e^{-\frac{1}{2}[(x_1-1)^2+(x_2-1)^2]}}
\\&=
\frac{\frac{1}{4\pi}e^{-\frac{1}{2}(x_1^2+x_2^2)}e^{x_1+x_2-1}}{\frac{1}{4\pi}e^{-\frac{1}{2}(x_1^2+x_2^2)}+\frac{1}{4\pi}e^{-\frac{1}{2}(x_1^2+x_2^2)}e^{x_1+x_2-1}}
\\&=
\frac{e^{x_1+x_2-1}}{1+e^{x_1+x_2-1}}
\\&=
\frac{1}{1 + e^{1-x_1-x_2}}
\end{align}
$$
Since $P(y = 0 \mid \mathtt {x};\space...) = 1 - P(y = 1 \mid \mathtt{x};\space...)$, we can get：
$$
\begin{align}
P(y=0\mid \mathtt{x};\phi,\mu_0, \mu_1, \Sigma_0, \Sigma_1)&=\frac{1}{1+e^{x_1+x_2-1}}
\end{align}
$$
Let $P(y=1\mid \mathtt{x};\phi,\mu_0, \mu_1, \Sigma_0, \Sigma_1) =P(y=0\mid \mathtt{x};\phi,\mu_0, \mu_1, \Sigma_0, \Sigma_1) $, We can get a plane:
$$
x_1 + x_2 = 1
$$
Since these two functions are monotonous with respect to $u=x1+x2$ , it is easy to know:

$\divideontimes$ if $x_1 +x_2 > 1, P(y = 1 \mid \mathtt{x}) > P(y = 0 \mid \mathtt{x});$

$\divideontimes$ if $x_1 +x_2 < 1, P(y = 1 \mid \mathtt{x}) < P(y = 0 \mid \mathtt{x});$

So that is the distribution plane. 



#### (b) 

Code Added (View $gaussian\_pos\_prob.m$ for details):

```matlab
M = size(X, 1);
for i = 1:N
    for j = 1:K
        x = X(:,i);
        mu = Mu(:,j);
        sigma = Sigma(:,:,j);
       P(i, j) = Phi(j) * exp((x-mu)' * sigma^-1 * (x-mu)/-2);
       P(i, j) = P(i,j)./ ((2 * pi)^(M/2) * sqrt(det(sigma)));
    end
end
P = P ./ sum(P,2);
p = P;
```



#### (c)

$N = 1000, Step = 0.01$

![](img\3c_1.png)

![](img\3c_2.png)

#### (d)

**[k-Classes Cases]**

Promote the case described in the problem to $k$ classes, $\forall {k}$:
$$
data_k=\left\{ (\mathtt{x}^{(i)},y^{(i)}) \mid y^{(i)}=k, i=1,\dots ,m\right\}
\\
\mathtt{x}_k=\left\{ \mathtt{x}^{(i)} \mid y^{(i)}=k, i=1,\dots ,m\right\}, N_k=\left|\mathtt{x}_k\right|
$$
According to the concept of MLE, We need to now:
$$
argmax({P\left(\mathtt{x}_k\mid \mathbf{\mu}_k, \mathbf{\Sigma}_k\right)})
$$
According to the formula in the slides:
$$
p(D\mid\theta) = \prod_{k = 1}p(\mathtt{x}_k \mid \theta)
$$
we get:
$$
P\left(\mathtt{x}_k\mid \mathbf{\mu}_k, \mathbf{\Sigma}_k\right) =\prod_{\mathtt{x}\in \mathtt{x}_k}{\frac{1}{(2\pi)^\frac{D}{2}\sqrt{|\mathbf{\Sigma}_k|}}e^{-\frac{1}{2}\left(\mathtt{x}-\mathbf{\mu}_k\right)^T\mathbf{\Sigma}_k^{-1}\left(\mathtt{x}-\mathbf{\mu}_k\right)}}
$$
To simplify the problem, we take the logarithm transformation:
$$
\begin{align}
\ln P\left(\mathtt{x}_k\mid \mathbf{\mu}_k, \mathbf{\Sigma}_k\right) = \ln(\prod_{\mathtt{x}\in \mathtt{x}_k}{\frac{1}{(2\pi)^{\frac{D}{2}}\sqrt{|\mathbf{\Sigma}_k|}}e^{-\frac{1}{2}\left(\mathtt{x}-\mathbf{\mu}_k\right)^T\mathbf{\Sigma}_k^{-1}\left(\mathtt{x}-\mathbf{\mu}_k\right)}})
\\= -\frac{1}{2}N_kD\ln{2\pi}-\frac{N_k}{2}\ln{\left|\mathbf{\Sigma}_k \right|}-\frac{1}{2}\sum_{\mathtt{x}\in \mathtt{x}_k}{\left(\mathtt{x}-\mathbf{\mu}_k\right)^T\mathbf{\Sigma}_k^{-1}\left(\mathtt{x}-\mathbf{\mu}_k\right)}
\\= -\frac{N_k}{2}(D\ln{2\pi}+\ln{\left|\mathbf{\Sigma}_k \right|})-\frac{1}{2}\sum_{\mathtt{x}\in \mathtt{x}_k}{\left(\mathtt{x}-\mathbf{\mu}_k\right)^T\mathbf{\Sigma}_k^{-1}\left(\mathtt{x}-\mathbf{\mu}_k\right)}
\end{align}
$$
**Partial derivative of $\mu_{k}$ (partly refer to slides):**
$$
\begin{align}
&&\frac{\partial}{\partial \mathbf{\mu}_k}\ln{P\left(\mathtt{x}_k\mid \mathbf{\mu}_k, \mathbf{\Sigma}_k\right)}&=\sum_{\mathtt{x}\in \mathtt{x}_k}{\mathbf{\Sigma}_k^{-1}\left(\mathtt{x}-\mathbf{\mu}_k\right)}=0\\
&&\boxed{
\mathbf{\mu}_{max}=\frac{\sum_{\mathtt{x}\in \mathtt{x}_k}{\mathtt{x}}}		{\left|\mathtt{x}_k\right|}
}
\end{align}
$$
**Partial derivative of $\mathbf{\Sigma}_k$(partly refer to slides):**
$$
\begin{align}
&&&\frac{\partial}{\partial \mathbf{\Sigma}_k}\ln{P\left(\mathtt{x}_k\mid \mathbf{\mu}_k, \mathbf{\Sigma}_k\right)}\\
\\&&=&
-\frac{N_k}{2}\mathbf{\Sigma}_k^{-1}-\frac{1}{2}\mathbf{\Sigma}_k^{-1}\left(\sum_{\mathtt{x}\in \mathtt{x}_k}{\left(\mathtt{x}-\mathbf{\mu}_k\right)\left(\mathtt{x}-\mathbf{\mu}_k\right)^T}\right)\mathbf{\Sigma}_k^{-1}
=0\\
\end{align}
$$
Sort the equation above, get：
$$
N_k\mathbf{\Sigma}_k^{-1}=\mathbf{\Sigma}_k^{-1}\left(\sum_{\mathtt{x}\in \mathtt{x}_k}{\left(\mathtt{x}-\mathbf{\mu}_k\right)\left(\mathtt{x}-\mathbf{\mu}_k\right)^T}\right)\mathbf{\Sigma}_k^{-1}\\
\boxed{
\mathbf{\Sigma}_{max}=\frac{\sum_{\mathtt{x}\in \mathtt{x}_k}{\left(\mathtt{x}-\mathbf{\mu}_{max}\right)\left(\mathtt{x}-\mathbf{\mu}_{max}\right)^T}}{\left|N_k\right|}
}
$$
Get:
$$
\mathbf{\mu}_{k}=\frac{\sum_{\mathtt{x}\in \mathtt{x}_k}{\mathtt{x}}}{\left|\mathtt{x}_k\right|}\\ 
\phi_k=\frac{\left|\mathtt{x}_k\right|}{\sum_{i=1}^K\left|\mathtt{x}_i\right|}
\\
\mathbf{\Sigma}_{k}=\frac{\sum_{\mathtt{x}\in \mathtt{x}_k}{\left(\mathtt{x}-\mathbf{\mu}_k\right)\left(\mathtt{x}-\mathbf{\mu}_k\right)^T}}{\left|\mathtt{x}_k\right|-1}\\
$$


### 4. Text Classification with Naïve Bayes



#### (a)

$Answer:$ By running corresponding code, we can know the order (viewed in MATLAB):

![](img\4a.png)

Look up the file, the top 10 word with the highest ratio is:

| 1    | 2      | 3     | 4      | 5    | 6    | 7    | 8         | 9    | 10     |
| ---- | ------ | ----- | ------ | :--- | ---- | ---- | --------- | ---- | ------ |
| nbsp | viagra | pills | cialis | voip | php  | meds | computron | sex  | ooking |

#### (b)

$Answer:$The accuracy is $0.9857$.(for calculation process see **(d)**)

#### (c)

$Answer:$ This statement is $FALSE$. According to Bayes theorem, if $p(Spam)=0.01, p(Ham)=0.99$,$S'$ or $H'$ means **considered** as spam & ham (but may not BE spam & ham). then:
$$
\begin{align}
P(S\mid S')=&\frac{P(S'\mid S)P(S)}{P(S'\mid S)P(S)+P(H'\mid H)P(H)}=0.5
\end{align}
$$

 The probability of precision is $0.5$. It's not a good ratio actually. So this statement is $FALSE$.

#### (d)

According to the output of the code(View $run.m$ for details):

```matlab
accuracy = (tp + tn) / (num_ham_test + num_spam_test);
precision = tp / (tp + fp);
recall = tp / (tp + fn);
fprintf("accuracy= %f.\n",accuracy);
fprintf("precision= %f.\n",precision);
fprintf("recall= %f.\n",recall);
```

![](img\4b.png)

The result is:
$$
\left\{
	\begin{aligned} 
    accurancy = 0.985732 \approx 0.9857 \\
    precision = 0.975022 \approx 0.9750 \\
    recall = 0.972420 \approx 0.9750 \\
    \end{aligned}
\right.
$$

#### (e)

$Answer:$ **Precision** should be the most important. Because it is basically acceptable that some spam is unshielded, but the result of missing important emails is devastating because of the high level of shielding. 

For airport security, however, **Recall** becomes especially important. There may be some passengers be misjudged, but for safety, it is always not too much to be careful.














