function add_uss = add_uss(A,B,C,D,E,Ts)

if(isempty(Ts))
    certain_ss = ss(A,B,C,D);
    add_ss = ss(A,E,C,0*E);
    add_uss.certain = certain_ss;
    add_uss.additive = add_ss;
    add_uss.matrices.A = A;
    add_uss.matrices.B = B;
    add_uss.matrices.C = C;
    add_uss.matrices.D = D;
    add_uss.matrices.E = E;
    add_uss.notes = 'Additive uncertain model of the form \dot{x} = A*x + B*u + E*w, Y = C*x + D*u';
else
    certain_ss = ss(A,B,C,D);
    certain_ss_d = c2d(certain_ss,Ts,'zoh');
    add_ss = ss(A,E,C,0*E);
    add_ss_d = c2d(add_ss,Ts,'zoh');
    add_uss.certain = certain_ss_d;
    add_uss.additive = add_ss_d;
    add_uss.Ts = Ts;
    add_uss.matrices.A = certain_ss_d.A;
    add_uss.matrices.B = certain_ss_d.B;
    add_uss.matrices.C = certain_ss_d.C;
    add_uss.matrices.D = certain_ss_d.D;
    add_uss.matrices.E = add_ss_d.B;
    add_uss.continuous.certain = certain_ss;
    add_uss.continuous.additive = add_ss;
    add_uss.continuous.matrices.A = A;
    add_uss.continuous.matrices.B = B;
    add_uss.continuous.matrices.C = C;
    add_uss.continuous.matrices.D = D;
    add_uss.continuous.matrices.E = E;
    add_uss.notes = 'Discretized Additive uncertain model of the form \dot{x} = A*x + B*u + E*w, Y = C*x + D*u';
end

end