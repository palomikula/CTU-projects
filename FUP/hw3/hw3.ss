#lang r5rs

;(require racket/trace)


(define (append2 a b)
  (cond ((null? a) b)
        ((not(pair? a)) (cons a b))
        (#t (cons (car a) (append2 (cdr a) b)))))

(define (is_equal? a b)
  (cond ((eqv? a b) #t)
        ((and (pair? a) (pair? b))
         (and (is_equal? (car a) (car b))
              (is_equal? (cdr a) (cdr b))))
        (#t #f)))

(define (get-nth ls n)
  (if (not (list? ls)) (begin (display ls) (newline) (display n) (newline) (car ls))
      (if (<= n 0) (car ls)
(get-nth (cdr ls) (- n 1)))))

(define (get_x state) (caadr state))

(define (get_y state) (cadadr state))

(define (get_row maze y help)
  (cond ((eqv? y help) (car maze))
        (#t (get_row (cdr maze) y (+ help 1)))))

(define (get_column row x help)
  (cond ((eqv? x help) (car row))
        (#t (get_column (cdr row) x (+ help 1)))))

(define (get_mark state)
  (get_column (get_row (car state) (get_y state) 0) (get_x state) 0))

(define (get_next_mark state)
  (cond ((eqv? (caddr state) 'south) (get_mark (list (car state) (list (caadr state) (+ (cadadr state) 1)) (caddr state))))
        ((eqv? (caddr state) 'east) (get_mark (list (car state) (list (+ (caadr state) 1) (cadadr state)) (caddr state))))
        ((eqv? (caddr state) 'north) (get_mark (list (car state) (list (caadr state) (- (cadadr state) 1)) (caddr state))))
        ((eqv? (caddr state) 'west) (get_mark (list (car state) (list (- (caadr state) 1) (cadadr state)) (caddr state))))
        (#t "invalid orientation")))

(define (north? state)
  (cond ((eqv? (caddr state) 'north) #t)
        (#t #f)))

(define (mark? state)
  (cond ((eqv? (get_mark state) 0) #f)
        (#t #t)))

(define (wall? state)
  (cond ((eqv? (get_next_mark state) 'w) #t)
        (#t #f)))

(define (is_command? a)
  (cond ((eqv? a 'step) #t)
        ((eqv? a 'turn-left) #t)
        ((eqv? a 'put-mark) #t)
        ((eqv? a 'get-mark) #t)
        (#t #f)))

(define (can_execute? a state)
  (cond ((eqv? a 'step) (if (wall? state) #f #t))
        ((eqv? a 'turn-left) #t)
        ((eqv? a 'put-mark) #t)
        ((eqv? a 'get-mark) (if (mark? state) #t #f))
        (#t "invalid command")))

(define (exec_command a state)
  (cond ((eqv? a 'step) (step state))
        ((eqv? a 'turn-left) (turn-left state))
        ((eqv? a 'put-mark) (put-mark state))
        ((eqv? a 'get-mark) (get-mark state))
        (#t "invalid command")))

(define (evaluate_cond expr state)
  (cond ((eqv? (car expr) 'wall?) (if (wall? state) (cadr expr) (caddr expr)))
        ((eqv? (car expr) 'north?) (if (north? state) (cadr expr) (caddr expr)))
        ((eqv? (car expr) 'mark?) (if (mark? state) (cadr expr) (caddr expr)))
        (#t "invalid condition")))

(define (plug_in_procedure expr program)
  (cond ((not(pair? expr)) (plug_in_procedure (list expr) program))
        ((null? program) expr)
        ((eqv? (cadar program) (car expr)) (cons (caddar program) (list 'end)))
        (#t (plug_in_procedure expr (cdr program)))))

(define (get-rows maze x y help new)
  (cond ((is_equal? maze (list)) new)
        ((eqv? y help) (get-rows(cdr maze) x y (+ help 1)(append2 new (list(get-missing-row (car maze) x 0 '())))))
        (#t (get-rows (cdr maze) x y (+ help 1) (append2 new (list(car maze)))))))

(define (get-missing-row row x help new)
  (cond ((eqv? row '()) new)
        ((eqv? x help) (get-missing-row (cdr row) x (+ help 1) (append2 new (list (- (car row) 1)))))
        (#t (get-missing-row (cdr row) x (+ help 1) (append2 new (list (car row)))))))

(define (put-rows maze x y help new)
  (cond ((is_equal? maze (list)) new)
        ((eqv? y help) (put-rows(cdr maze) x y (+ help 1)(append2 new (list(put-missing-row (car maze) x 0 '())))))
        (#t (put-rows (cdr maze) x y (+ help 1) (append2 new (list(car maze)))))))

(define (put-missing-row row x help new)
  (cond ((eqv? row '()) new)
        ((eqv? x help) (put-missing-row (cdr row) x (+ help 1) (append2 new (list (+ (car row) 1)))))
        (#t (put-missing-row (cdr row) x (+ help 1) (append2 new (list (car row)))))))

(define (turn-left state)
  (cond ((eqv? (caddr state) 'south) (list (car state) (cadr state) 'east))
        ((eqv? (caddr state) 'east) (list (car state) (cadr state) 'north))
        ((eqv? (caddr state) 'north) (list (car state) (cadr state) 'west))
        ((eqv? (caddr state) 'west) (list (car state) (cadr state) 'south))
        (#t "invalid orientation")))

(define (get-mark state)
  (append2(list(cdr(get-rows (car state) (get_x state) (get_y state) 0 (list(list)))))(cdr state)))

(define (put-mark state)
  (append2(list(cdr(put-rows (car state) (get_x state) (get_y state) 0 (list(list)))))(cdr state)))

(define (step state)
   (cond ((eqv? (caddr state) 'south) (list (car state) (list (caadr state) (+ (cadadr state) 1)) (caddr state)))
         ((eqv? (caddr state) 'east) (list (car state) (list (+ (caadr state) 1) (cadadr state)) (caddr state)))
         ((eqv? (caddr state) 'north) (list (car state) (list (caadr state) (- (cadadr state) 1)) (caddr state)))
         ((eqv? (caddr state) 'west) (list (car state) (list (- (caadr state) 1) (cadadr state)) (caddr state)))
         (#t "invalid orientation")))

(define (help_simulate state expr program limit count sequence seq-threshold)
  ;(display count)
  ;(display "\n")
  (cond ((eqv? expr '()) (list sequence state))
        ((= limit count) (list sequence state))
        ((> 0 seq-threshold) (list sequence state))
        ((not(pair? expr)) (help_simulate state (list expr) program limit count sequence seq-threshold))
        ((pair? (car expr)) (help_simulate state
                                           (if (null? (cdar expr))
                                               (append2 (list (caar expr))(cdr expr))
                                               (append2 (append2 (list (caar expr))(list(cdar expr))) (cdr expr))
                                           )
                                           program
                                           limit
                                           count
                                           sequence
                                           seq-threshold))

        ((null? (car expr)) (help_simulate state (cdr expr) program limit count sequence seq-threshold))

        ((is_command? (car expr)) (if (can_execute? (car expr) state)
                                      (help_simulate (exec_command (car expr) state)
                                                     (cdr expr)
                                                     program
                                                     limit
                                                     count
                                                     (append2 sequence (list (car expr)))
                                                     (- seq-threshold 1))
                                      (list sequence state)))

        ((eqv? (car expr) 'if) (help_simulate state
                                           (cons (evaluate_cond (cadr expr) state) (cddr expr))
                                           program
                                           limit
                                           count
                                           sequence
                                           seq-threshold))

        ((eqv? (car expr) 'end)  (help_simulate state
                                           (cdr expr)
                                           program
                                           limit
                                           (- count 1)
                                           sequence
                                           seq-threshold))
        (#t (help_simulate state
                           (cons (plug_in_procedure expr program) (cdr expr))
                           program
                           limit
                           (+ count 1)
                           sequence
                           seq-threshold))
                       )

                   
            
       
  )


;(trace help_simulate)

(define (simulate state expr program limit seq-threshold)
  (help_simulate state (if (pair? expr) expr (list expr)) program limit -1 '() seq-threshold))

(define (maze-dist state1 state2) (maze-dist-help (car state1) (car state2) 0))

(define (maze-dist-help maze1 maze2 count)
  (cond ((null? maze1) count)
        ((null? (car maze1)) (maze-dist-help (cdr maze1) (cdr maze2) count))
        ((eqv? (caar maze1) 'w) (maze-dist-help (cons (cdar maze1) (cdr maze1))
                                                  (cons (cdar maze2) (cdr maze2))
                                                  count))
        (#t (maze-dist-help (cons (cdar maze1) (cdr maze1))
                             (cons (cdar maze2) (cdr maze2))
                             (+(abs(- (caar maze1) (caar maze2))) count)))
        )
  )

(define (state-dist state1 state2)
  (let ((md (+(abs(- (caadr state1) (caadr state2)))(abs(- (cadadr state1) (cadadr state2))))))
    (if (eqv?(caddr state1)(caddr state2)) md (+ md 1))
    ))

(define (prg-length prg) (help-prg-length prg 0))

(define (add-lists . lists)
  (apply map + lists))

(define (help-prg-length prg length)
  (cond ((null? prg) length)
        ((pair? (car prg))(help-prg-length (if (null? (cdar prg))
                                               (cons (caar prg)(cdr prg))
                                               (append2 (cons(caar prg)(list(cdar prg))) (cdr prg)))
                                           length))
        ((null? (car prg))(help-prg-length (cdr prg) length))
        ((eqv? (car prg) 'if)(help-prg-length (cdr prg) length))
        ((eqv? (car prg) 'procedure)(help-prg-length (cdr prg) length))
        (#t (help-prg-length (cdr prg) (+ length 1)))))

(define (admissible? test threshold)
  (cond ((null? test) #t)
        ((< (car threshold)(car test)) #f)
        (#t (admissible? (cdr test) (cdr threshold)))))

(define (program-fitness prg pop-pairs threshold limit fitness)
  (cond ((not(admissible? fitness threshold)) 'nope)
        ((null? pop-pairs) (cons fitness (list prg)))
        (#t(let ((output (simulate (caar pop-pairs) '(start) prg limit (cadddr threshold))))
             (let*(
                   (md (maze-dist (cadr output) (cadar pop-pairs)))
                   (sd (state-dist (cadr output) (cadar pop-pairs)))
                   (pl (if (= (caddr fitness) 0)(prg-length prg)0))
                   (ns (prg-length (car output)))
                   )
               (program-fitness prg (cdr pop-pairs) threshold limit (add-lists (list md sd pl ns) fitness)))))))

(define (help-insert old ins new)
  (cond ((null? old) (append2 new (list ins)))
        ((null? (car old)) (list ins))
        ((< (caar ins) (caaar old))(append2 new (cons ins old)))
        ((> (caar ins) (caaar old))(help-insert (cdr old) ins (append2 new (list(car old)))))
        (#t (cond((< (cadar ins) (cadaar old))(append2 new (cons ins old)))
                 ((> (cadar ins) (cadaar old))(help-insert (cdr old) ins (append2 new (list(car old)))))
                 (#t (cond((< (caddar ins) (car(cddaar old)))(append2 new (cons ins old)))
                          ((> (caddar ins) (car(cddaar old)))(help-insert (cdr old) ins (append2 new (list(car old)))))
                          (#t (cond((< (car(cdddar ins)) (cadr(cddaar old)))(append2 new (cons ins old)))
                                   ((> (car(cdddar ins)) (cadr(cddaar old)))(help-insert (cdr old) ins (append2 new (list(car old)))))
                                   (#t (append2 new (cons ins old)))))))))))

(define (insert to ins) (help-insert to ins '()))
                                   
(define (help-evaluate progs pop-pairs threshold limit result)
  ;(display result)
  ;(display '(*************************************************))
  (cond ((null? progs) result)
        (#t (let ((fitness (program-fitness (car progs) pop-pairs threshold limit '(0 0 0 0))))
              (if (eqv? fitness 'nope)
                  (help-evaluate (cdr progs) pop-pairs threshold limit result)
                  (help-evaluate (cdr progs) pop-pairs threshold limit (insert result fitness)))))))
  
(define (evaluate progs pop-pairs threshold limit)
  (help-evaluate progs pop-pairs threshold limit '(())))

;__________________________________________________________
(define (is-less? elem other)
  (cond
    ((null? elem) #f)
    ((< (car elem) (car other)) #t)
    ((> (car elem) (car other)) #f)
(else (is-less? (cdr elem) (cdr other)))))

(define (congruential-rng seed)
  (let ((a 16807 #|(expt 7 5)|#)
        (m 2147483647 #|(- (expt 2 31) 1)|#))
    (let ((m-1 (- m 1)))
      (let ((seed (+ (remainder seed m-1) 1)))
        (lambda (b)
          (let ((n (remainder (* a seed) m)))
            (set! seed n)
            (quotient (* (- n 1) b) m-1)))))))
(define random (congruential-rng 12345))


(define (get-procedure-list program)
  (define (get-procedure-list-inner ls prog)
    (if (null? prog) ls
        (get-procedure-list-inner (cons (cadar prog) ls) (cdr prog))))
  (get-procedure-list-inner '() program))

(define (contains? ls elem)
  (if (null? ls) #f
      (if (equal? (car ls) elem)
          #t
          (contains? (cdr ls) elem)
          )))


(define (validate prog)
  ;(display "Validating: ") (display prog) (newline) (flush-output)
  (define (remove-additional-brackets prog)
    (if (null? prog) prog
        (if (not (list? prog)) prog
            (if (list? (car prog))
                (if (null? (car prog)) (remove-additional-brackets (cdr prog))
                    (if (equal? (caar prog) 'if)
                        (cons (list 'if (cadar prog)
                                    (if (not (list? (caddar prog))) (list (remove-additional-brackets (caddar prog)))
                                        (remove-additional-brackets (caddar prog)))
                                    (if (not (list? (car (cdddar prog)))) (list (remove-additional-brackets (car (cdddar prog))))
                                        (remove-additional-brackets (car (cdddar prog)))))
                              (remove-additional-brackets (cdr prog)))
                        (remove-additional-brackets (append (car prog) (cdr prog)))))
                (if (equal? (car prog) 'if)
                    (list 'if (cadr prog)
                          (if (not (list? (caddr prog))) (list (caddr prog))
                              (remove-additional-brackets (cadddr prog)))
                          (if (not (list? (cadddr prog))) (list (cadddr prog))
                              (remove-additional-brackets (cadddr prog))))
                         

                    (cons (car prog) (remove-additional-brackets (cdr prog))))))))
  
  (define (remove-empty-procedures prog)

    (define (remove-procedure name)

      (define (inner left right)

        (define (inner-procedure procedure)
          (if (null? procedure) procedure
              (if (not (list? procedure))
                  (if (equal? name procedure) '() procedure)
                  (if (null? (car procedure)) (cons '() (inner-procedure (cdr procedure)))
                      (if (list? (car procedure)) (cons (inner-procedure (car procedure)) (inner-procedure (cdr procedure)))
                          (if (equal? (car procedure) name) (inner-procedure (cdr procedure))
                              (cons (car procedure) (inner-procedure (cdr procedure)))))))))
          
        (if (null? right) (remove-empty-procedures left)
            (if (equal? (cadar right) name) (inner left (cdr right))
                (inner (cons (list 'procedure (cadar right) (inner-procedure (caddar right))) left) (cdr right)))))
      (inner '() prog))
    
    (define (browse-procedures procedures)
      (if (null? procedures) prog
          (if (equal? (cadar procedures) 'start) (browse-procedures (cdr procedures))
              (if (null? (caddar procedures)) (remove-procedure (cadar procedures))
                  (browse-procedures (cdr procedures))))))
    
    (browse-procedures prog))
  
  (define (for-each-procedure prog call)
    (if (null? prog) prog
        (cons (list 'procedure (cadar prog)
                    (if (not (list? (caddar prog)))
                        (list (caddar prog))
                        (if (null? (caddar prog)) '()
                            (if (equal? (car (caddar prog)) 'if) (call (list (caddar prog))) (call (caddar prog))))))
              (for-each-procedure (cdr prog) call))))
  
  (for-each-procedure (remove-empty-procedures (for-each-procedure prog remove-additional-brackets)) remove-additional-brackets)
)

(define (mutate program)
  (validate (call-mutate (validate program))))

(define (call-mutate program)
  
  (define (get-highest-procedure)
    (define (inner ls)
      (if (null? ls) 0
          (if (not (number? (car ls))) (inner (cdr ls))
              (let ((result (inner (cdr ls))))
                (if (> (car ls) result) (car ls) result)))))
    (inner (get-procedure-list program)))
  
  (define (add)  
    (define (add-call)
      (define (add-procedure-call)
        (define (get-procedure-from-list ls index)
          (if (= index 0) (car ls)
              (get-procedure-from-list (cdr ls) (- index 1))))
        
        (let ((procedure-list (get-procedure-list program)))
          (get-procedure-from-list procedure-list (random (length procedure-list)))))
      
      (define (add-if)
        (define (get-question)
          (let ((rand (random 7)))
            (cond
              ((< rand 3) 'wall?)
              ((< rand 5) 'north?)
              (else 'mark?))))
        (if (= 0 (random 4))
            '(if wall? (turn-left) (step))
            (list 'if (get-question) (list (add-call)) (list (add-call)))))
      
      (let ((rand (random 16)))
        (cond
          ((< rand 6) 'step)
          ((< rand 9) 'turn-left)
          ((= rand 9) (add-procedure-call))
          ((< rand 11) 'put-mark)
          ((< rand 13) 'get-mark)
          (else (add-if)))))
    
    (define (add-procedure)
      ;Create function with random call
      ;Place function call at random place of 1st level
      (define (place-call func-name program)
        (define (place-in-procedure procedure)
          (define (goto-place prog index)
            (if (= index 0) (cons (inner (car prog)) (cdr prog))
                (cons (car prog) (goto-place (cdr prog) (- index 1)))))
            
          (define (inner prog)
            (if (null? prog) func-name
                (if (not (list? prog))
                    (if (= (random 2) 0) (list prog func-name) (list func-name prog))
                    (if (equal? (car prog) 'if)
                        (if (= (random 2) 0) (list 'if (cadr prog) (inner (caddr prog)) (cadddr prog)) (list 'if (cadr prog) (caddr prog) (inner (cadddr prog))))
                        (goto-place prog (random (length prog)))))))
          
          (list 'procedure (cadr procedure) (inner (caddr procedure))) 
          )
        (define (select-procedure)
          (define (inner prog index)
            (if (= index 0) (cons (place-in-procedure (car prog)) (cdr prog))
                (cons (car prog) (inner (cdr prog) (- index 1)))))
          (inner program (random (length program))))
        (select-procedure))
      
      (define (createProcedure)
        (let ((name (+ (get-highest-procedure) 1)))
          (list name (append program (list (list 'procedure name (list (add-call))))))))
      
      (let* ((result (createProcedure))
             (name (car result))
             (new-prog (cadr result)))
        (place-call name new-prog))
      )
    
    (define (add-element)
      (define (find-place procedure) ;Calls add-call at random point of program
        (define (find-place-inner rest)
          (define (handle-condition rest)
            (if (= (random 2) 0)
                (append (list (car rest) (cadr rest) (find-place-inner (caddr rest))) (cdddr rest))
                (append (list (car rest) (cadr rest) (caddr rest) (find-place-inner (cadddr rest))) (cddddr rest))))
          
          (define (goto-place rest index)
            (if (= index 0)
                (if (list? (car rest))
                    (let ((rand (random 4)))
                      (cond
                        ((= rand 0) (cons (add-call) rest))
                        ((= rand 1) (cons (car rest) (cons (add-call) (cdr rest))))
                        (else (cons (handle-condition (car rest)) (cdr rest)))))
                    (if (= (random 2) 0)
                        (cons (add-call) rest)
                        (cons (car rest) (cons (add-call) (cdr rest)))))
                (cons (car rest) (goto-place (cdr rest) (- index 1)))))
          
          (if (null? rest)
              (add-call)
              (if (list? rest)
                  (if (equal? (car rest) 'if)
                      (let ((rand (random 4)))
                        (cond
                          ((= rand 0) (list (add-call) rest))
                          ((= rand 1) (list rest (add-call)))
                          (else (handle-condition rest)) ))
                      (goto-place rest (random (length rest))))
                  (if (= (random 2) 0)
                      (list (add-call) rest)
                      (list rest (add-call))))))
        
        
        (list (car procedure) (cadr procedure) (find-place-inner (caddr procedure))))
      
      (define (select-procedure) ;Calls find-place on randomly chosen procedure
        
        (define (select-procedure-inner rest index)
          (if (= 0 index)
              (cons (find-place (car rest)) (cdr rest))
              (cons (car rest) (select-procedure-inner (cdr rest) (- index 1)))))
        
        (select-procedure-inner program (random (length program))))
      
      (select-procedure))
    
    (if (= (random 6) 0)
        (add-procedure)
        (add-element)))

  (define (remove)
  
    (define (find-call procedure)
      
      (define (stand-on procedure)
        (cond
          ((null? procedure) procedure)
          ((not (list? procedure)) '())
          ((equal? 'if (car procedure))
           (if (= (random 2) 0)
               (list 'if (cadr procedure) (go-to (caddr procedure) (random (length (caddr procedure)))) (cadddr procedure))
               (list 'if (cadr procedure) (caddr procedure) (go-to (cadddr procedure) (random (length (cadddr procedure)))))))
          ((list? (car procedure))
           (if (= (random 5) 0) (cdr procedure)
               (if (null? (car procedure)) '()
                   (if (equal? (caar procedure) 'if) (cons (stand-on (car procedure)) (cdr procedure))
                       (go-to (car procedure) (random (length (car procedure))))))))
           
          ((not (list? (car procedure))) (cdr procedure))
          (else (display "WTF: ") (display procedure) (newline)
           
                )))
      
      (define (go-to procedure index)
        (if (null? procedure) procedure
            (if (equal? (car procedure) 'if) (stand-on procedure)
                (if (= index 0) (stand-on procedure)
                    (cons (car procedure) (go-to (cdr procedure) (- index 1)))))))
      
      (if (list? procedure) (go-to procedure (random (length procedure)))
          '()))
      
      
    
    (define (select-procedure prog index)
      (if (= index 0) (cons (list 'procedure (cadar prog) (find-call (caddar prog))) (cdr prog))
          (cons (car prog) (select-procedure (cdr prog) (- index 1)))))

    (select-procedure program (random (length program))))
  
  (if (null? program) '((procedure start (if north? (step start) (turn-left start))))
      (if (> (random 2) 0)
          (validate (add))
(validate (remove)))))

(define (evolve pairs thresholds stack-size)
  (define (POPSIZE) 600)
  (define (create-initial)
    (define (get-start) '(
                          ((procedure start (step)))
                          ((procedure start ((if wall? (turn-left) (step)))))
                          ((procedure start ((if mark? () (1)) 1 step step step)) (procedure 1 (put-mark get-mark step step step)))
                          ((procedure start (turn-right (if wall? (turn-left (if wall? (turn-left (if wall? turn-left step)) step)) step) put-mark start))
                           (procedure turn-right (turn-left turn-left turn-left)))
                          ((procedure turn-north ((if north? () (turn-left turn-north)))))
                          ((procedure add-one ((if mark? (get-mark step add-one turn-180 step turn-180) (put-mark)))) 
                           (procedure turn-180 (turn-left turn-left)))
                          ((procedure start ((if wall? (turn-left turn-left) (step start step)))))
                          ((procedure start ((if mark? (get-mark step start turn-180 step turn-180) (put-mark))) (procedure turn-180 (turn-left turn-left))))
                          ((procedure start ((if wall? put-mark step))))
                          ((procedure start fill-maze)
                           (procedure fill-maze ((if mark? () (put-mark (if wall? () (step fill-maze step-back)) turn-left (if wall? () (step fill-maze step-back)) turn-left turn-left (if wall? () (step fill-maze step-back)) turn-left))))
                           (procedure step-back (turn-left turn-left step turn-left turn-left)))
                          ((procedure start ((if wall? () (2)) put-mark)) (procedure 2 (step start step)))
                          ((procedure start (step step step put-mark)))
                          ((procedure start (put-mark (if wall? (turn-left) (step)) start)))
                          ((procedure start (turn-north go)) (procedure go ((if wall? () (step go)))) (procedure turn-north ((if north? () (turn-left turn-north)))))
                          ((procedure start (go-north go)) (procedure go ((if wall? (turn-left go) (step go-north go)))) (procedure go-north ((if north? () (turn-left go-north)))))
                          ((procedure start (put-mark (if mark? (turn-left turn-left) ()) step put-mark)))
                          ((procedure start (() turn-left turn-left turn-left)))
                          ((procedure start ((if north? (start) ()) turn-left start)))
                          ((procedure start ((if wall? () (2)) put-mark)) (procedure 2 (step start step)))
                          ((procedure start (step step step put-mark)))
                          ((procedure start (put-mark (if wall? (turn-left) (step)) start)))
                          ((procedure start ((if wall? (put-mark) (step start turn-left turn-left step turn-left turn-left)))))
                          ((procedure start ((if wall? (put-mark) (step start)))))
                          ((procedure start put-mark))
                          ((procedure start ((if wall? (put-mark) (step)))))
                          ((procedure start (go-north go)) (procedure go ((if wall? (turn-left go) (step go-north go)))) (procedure go-north ((if north? () (turn-left go-north)))))
                          ((procedure start (turn-north go)) (procedure go ((if wall? () (step go)))) (procedure turn-north ((if north? () (turn-left turn-north)))))
                          ((procedure start (if wall? (put-mark) (step start turn-left turn-left step turn-left turn-left))))
                          ))
    
    (define (load-start population)
      (if (> (length population) (POPSIZE)) (load-start (cdr population))
          (if (= (length population) (POPSIZE)) population
              (load-start (append population (get-start))))))
    (load-start '()))
  
  (define (get-next-pop population old-pop)
    (display 'next)
    (define (cut percentage) ;Cuts bottom [percentage] percent of programs
      (define (inner n threshold prog)
        (if (<= n threshold) '()
            (cons (car prog) (inner (- n 1) threshold (cdr prog)))))
      (inner (length population) (* (POPSIZE) (/ percentage 100)) population))
    
    
    (define (get-from-percentage percentage)
      (let* ((res (cut percentage))
             (which (random (length res)))
             (output (get-nth res which)))
        (if (null? output) (mutate '((procedure start (turn-right step)) (procedure turn-right (turn-left turn-left turn-left))))
            output)))
    
    (define (next-wave new-population len)
      (if (>= len (POPSIZE)) (let ((result (map mutate new-population))) (if (= (random 6) 0) (map mutate result) result))
          (next-wave (cons (get-from-percentage (* (random 10) 10)) new-population) (+ len 1))))
    
    (if (< (length population) (POPSIZE)) (get-next-pop (cons (get-nth old-pop (random (length old-pop))) population) old-pop) 
        (next-wave '() 0)))
  
  (define (eval-pop population best)
    
    (let* ((result (evaluate population pairs thresholds stack-size))
           (best-score (caar result))
           (new-pop (map (lambda (n) (cadr n)) result))
           (best-code (car new-pop)))
      (if (and (not (equal? best-code '((procedure start ())))) (not (null? best-code)) (is-less? best-score best))
          (begin (display (list best-score best-code)) (newline) (flush-output) (eval-pop (get-next-pop new-pop population) best-score))
          (eval-pop (get-next-pop new-pop population) best))))
  
(eval-pop (create-initial) '(9999999 0 0 0)))

(define (get-highest-procedure program)
    (define (inner ls)
      (if (null? ls) 0
          (if (not (number? (car ls))) (inner (cdr ls))
              (let ((result (inner (cdr ls))))
                (if (> (car ls) result) (car ls) result)))))
    (inner (get-procedure-list program)))

(define pairs
    '(
      (
       (((w w w) 
         (w 1 w) 
         (w 1 w) 
         (w w w))  
        (1 2) south)
       
       (((w w w) 
         (w 2 w) 
         (w 2 w) 
         (w w w))  
        (1 1) north)
       )
      (
       (((w w w w w w) 
         (w 0 0 0 0 w) 
         (w 0 0 0 0 w) 
         (w 0 0 0 0 w) 
         (w 0 0 0 w w) 
         (w w w w w w)) 
        (2 3) north)
       
       (((w w w w w w) 
         (w 0 0 0 0 w) 
         (w 0 0 0 0 w) 
         (w 0 1 0 0 w) 
         (w 0 1 0 w w) 
         (w w w w w w)) 
        (2 4) south)
       )
      (
       (((w w w w w w) 
         (w 0 0 0 0 w) 
         (w 0 0 3 1 w) 
         (w 1 3 0 0 w) 
         (w w w w w w)) 
        (2 2) east)
       
       (((w w w w w w) 
         (w 0 0 0 0 w) 
         (w 1 1 3 1 w) 
         (w 1 3 0 0 w) 
         (w w w w w w))
        (1 2) west)
       )
      (
       (((w w w w w w) 
         (w 3 3 0 0 w) 
         (w w w w w w)) 
        (3 1) west)
       
       (((w w w w w w) 
         (w 3 3 1 1 w) 
         (w w w w w w))
        (4 1) east)
       )
      )
    )
