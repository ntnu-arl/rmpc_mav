function [H,K,F,G,Nc] = GetRobustMPC_Matrices(sol_mp)
%
%   Get the H, K, F, G, Nc parameters required for the simulink-based AND
%   auto-code generation (matlab and c code) of our Robust MPC framework
%
%   Syntax: 
%   [H,K,F,G,Nc] = GetRobustMPC_Matrices(sol_mp)
%
%   Inputs:
%   sol_mp          : The multiparametrically computed robust mpc 
%
%   Outputs:
%   H, K, F, G, Nc matrices 
%
%   Important note: the matrices now match the structure that a classical
%   LTI explicit MPC would produce using the MPT toolbox. This is very good
%   for compatibility issues
%
%   Additional note: this function seems messy due to several compatibility
%   issues between the YALMIP classes and the exact form of the MPT object
%   classes. However, it is thought that making our robust mpc compatible
%   with all the code generation of mpt makes sense.
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

nr = length(sol_mp.dynamics);

Pn = sol_mp.Pn;
Pn_orig = Pn;
Pn_plh = [];

% for kk = 1:length(Pn)


for kk = 1:length(Pn)
    [HH,KK] = double(Pn.A);
    [HH,KK] = double(Pn(kk));
    Pn_plh = [Pn_plh; Polyhedron(HH,KK)];
end

Pn = Pn_plh

%   extract hyperplane representation
Hn = cell(nr,1);
Kn = cell(nr,1);

[H_A,K_A] = double(Pn_orig.A);
Pn_A = Polyhedron(H_A,K_A);
Pn_A.computeVRep();
Pn_A_plt = polytope(H_A,K_A);
[H_b,K_b] = double(Pn_orig.b);
Pn_b = Polyhedron(H_b,K_b);
Pn_b.computeVRep();
Pn_b_plt = polytope(H_b,K_b);

[Hn{:}] = deal(Pn.A);
[Kn{:}] = deal(Pn.b);

[Hn{:}] = deal(Pn_plh.A);
[Kn{:}] = deal(Pn_plh.b);

Hn_plt = cell(nr,1);
Kn_plt = cell(nr,1);
[Hn_plt{:}] = deal(Pn_A_plt);
[Kn_plt{:}] = deal(Pn_b_plt);

if ~iscell(Hn),
    Hn = {Hn};
    Kn = {Kn};
end

%   extract control law
Fi = sol_mp.Fi;
Gi = sol_mp.Gi;

Nc = zeros(nr,1);

for i = 1:nr;
    Nc(i) = size(Hn{i},1);
end

%   count numer of constraints
nctotal = 0;
for ii = 1:nr
    nctotal = nctotal + size(Pn(ii).H,1);
end

nx = length(sol_mp.Fi{1}); % this should give us the number of states

H = zeros(nctotal,nx);
K = zeros(nctotal,1);
F = zeros(nr,nx);
G = zeros(nr,1);

abspos = 1;
for i = 1:nr
    H(abspos:abspos+Nc(i)-1,:) = Hn{i};
    K(abspos:abspos+Nc(i)-1,:) = Kn{i};
    abspos = abspos + Nc(i);
    
    F(i,:) = Fi{i};
    G(i,:) = Gi{i};
end

end