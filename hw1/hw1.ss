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

(define (help_simulate state expr program limit count sequence)
  (cond ((eqv? expr '()) (list sequence state))
        ((= limit count) (list sequence state))
        ((not(pair? expr)) (help_simulate state (list expr) program limit count sequence))
        ((pair? (car expr)) (help_simulate state
                                           (if (null? (cdar expr))
                                               (append2 (list (caar expr))(cdr expr))
                                               (append2 (append2 (list (caar expr))(list(cdar expr))) (cdr expr))
                                           )
                                           program
                                           limit
                                           count
                                           sequence))

        ((null? (car expr)) (help_simulate state (cdr expr) program limit count sequence))

        ((is_command? (car expr)) (if (can_execute? (car expr) state)
                                      (help_simulate (exec_command (car expr) state)
                                                     (cdr expr)
                                                     program
                                                     limit
                                                     count
                                                     (append2 sequence (list (car expr))))
                                      (list sequence state)))

        ((eqv? (car expr) 'if) (help_simulate state
                                           (cons (evaluate_cond (cadr expr) state) (cddr expr))
                                           program
                                           limit
                                           count
                                           sequence))

        ((eqv? (car expr) 'end)  (help_simulate state
                                           (cdr expr)
                                           program
                                           limit
                                           (- count 1)
                                           sequence))
        (#t (help_simulate state
                           (cons (plug_in_procedure expr program) (cdr expr))
                           program
                           limit
                           (+ count 1)
                           sequence))
                       )

                   
            
       
  )

;(trace help_simulate)

(define (simulate state expr program limit)
  (help_simulate state (if (pair? expr) expr (list expr)) program limit -1 '()))