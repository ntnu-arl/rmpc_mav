%%  Simple Linear Programming Examples

sdpvar x w
F = [x + w <= 1];
W = [-0.5 <= w < 0.5, uncertain(w)];
objective = -x;

sol = solvesdp(F+W, objective)

 

sdpvar x w t1 t2
F = [x+w <= 1];
W = [w== t1*(-0.5) + t2*0.5, t1+t2 == 1, 0 <= [t1 t2] <= 1];
W = [W,uncertain([t1 t2 w])];
objective = -x;
sol = solvesdp(F+W,objective)
%   in this case, the explicit norm-ball filter cannot be invoked, and the
%   enumeration filter is used. If you want to try the duality filter,
%   simply change an option

sdpvar x w t1 t2
F = [x+w <= 1];
W = [w == t1*(-0.5) + t2*0.5, t1+t2 == 1, 0 <= [t1 t2] <= 1]
W = [W,uncertain([t1 t2 w])];
objective = -x;
sol = solvesdp(F+W,objective,sdpsettings('robust.lplp','duality'));

%%  Conic uncertainty models
%
%   The uncertainty description has to be conic representable (which in
%   YALMIP means that it only uses linear conic constraints, or nonlinear
%   operators that can be represented using linear conic constraints).
%
%   The following more advanced model should have an optimal objective of 0

sdpvar x w(2,1)
F = [x+sum(w) <=1];
W = [norm(w) <= 1/sqrt(2), uncertain(w)];
objective = -x;
sol = solvesdp(F+W,objective);
%
%   The following alternative model is also valid
%
sdpvar x w(2,1)
F = [x+sum(w) <= 1];
W = [w'*w <= 1/2, uncertain(w)];
objective = -x;
sol = solvesdp(F+W,objective)

%%  More Advanced Problems
%
%   The robustification is done using strong duality arguments in the
%   uncertainty space, or by simple enumeration. This means that there is
%   no problemwith integer and logic constraints in the decision variables.

sdpvar x w 
F = [x+w <= 1, ismember(x,[0.2 0.4])];
W = [-0.5 <= w <= 0.5, uncertain(w)];
objective = -x; 
sol = solvesdp(F+W,objective)
%
%   To illustrate even stronger how transparently integrated the robust
%   optimization framework is, we solve an uncertain sum-of-squares problem
%   involving a nonlinear operator leading to integrality constraints, thus
%   forcing the resulting SDP to be solved using the internal mixed-integer
%   SDP solver.
%
%   We want to find an integer value a, taking values in the range from 3
%   to 5, such that the polynomial ax^4 + y^4 + uxy + 1 has a lower bound
%   larger than t, for any u in the range -1 to 1. A trivial and contrived
%   example of course, but here is the compact YALMIP model. 

sdpvar x y t u a
p = a*x^4 + y^4 + u*x*y + 1;
F = [uncertain(u), -1 <= u <= 1];
F = [F, ismember(a,[3 4 5])];
F = [F, sos(p-t)];
solvesos(F,-t)

%%  Uncertain semidefinite and second order cone constraints
%
%   YALMIP can robustify second order cone (SOCP) constraints with affine
%   dependence on the uncertainty, under the restriction that the
%   uncertainty is constrained to a polytopic set. In this case, YALMIP
%   derives a robust counterpart by enumerating the vertices of the
%   uncertainty set and define a new SOCP or SDP constraint for every
%   vertex. For the enumeration to work, you have to have MPT installed.
%
%   The following code computes a common Lyapunov function for a system
%   with uncertain dynamics due to an unknown parameter t. 

t = sdpvar(1,1);
P = sdpvar(2,2);
A = [-1 t; 0.2 -1]
F = [A'*P + P*A <= -eye(2), 1 <= t <= 2, uncertain(t)];
sol = solvesdp(F,trace(P));

%%  Useful features
%   
%   In some cases, we only want to derive the robust counter-part, but not
%   actually solve it. This can be accomplished with the command robustify.
%

[Frobust,robust_objective] = robustify(F+W,objective);

%   The robustified model can be manipulated as any other YALMIP model and
%   can of course be solved

sol = solvesdp(Frobust,robust_objective);
