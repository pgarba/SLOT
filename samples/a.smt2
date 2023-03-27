(set-info :smt-lib-version 2.6)
(set-logic QF_LIA)
(set-info :source | Leonardo de Moura |)
(set-info :category "check")
(set-info :status unsat)
(set-info :notes |This benchmark is designed to check if the integer DP is complete.|)
(declare-fun x1 () Int)
(declare-fun x2 () Int)
(declare-fun x3 () Int)
(declare-fun x4 () Int)
(declare-fun z () Bool)
(assert (or (> (+ x1 x2) (mod (abs x2) 20)) z (and (> -15 (- x3 x4)) (> 5 (ite (<= x1 3) (* x1 4) (div x1 4))))))
(check-sat)
(exit)
