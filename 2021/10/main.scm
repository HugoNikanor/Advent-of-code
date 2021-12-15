(use-modules (srfi srfi-41)
             (srfi srfi-1)
             (ice-9 rdelim)
             (ice-9 streams)
             (ice-9 control))

(define (flip token)
  (case token
    ((#\() #\))
    ((#\)) #\()
    ((#\<) #\>)
    ((#\>) #\<)
    ((#\[) #\])
    ((#\]) #\[)
    ((#\{) #\})
    ((#\}) #\{)))

(define (lookup-points token)
  (case token
    ((#t #f) 0)
    ((#\)) 3)
    ((#\]) 57)
    ((#\}) 1197)
    ((#\>) 25137)))

(define scores '())

(stream->list
 (stream-map
  (lambda (line)
    (define tokens (string->list line))
    (format #t "~%~a " line)
    (call/ec
     (lambda (return)
       (define stk
        (fold (lambda (token stack)
                (if (memv token '(#\{ #\( #\[ #\<))
                    (cons token stack)
                    ;; else closing token
                    (if (null? stack)
                        ;; too many closing parenthesee
                        (return #f)
                        (if (char=? (flip token)
                                    (car stack))
                            ;; correct ending token, pop it off the stack
                            (cdr stack)
                            ;; incorrect ending
                            (begin
                              (format #t "Expected `~a`, but found `~a` instead."
                                      (flip (car stack)) token)
                              (return token))))))
              '() tokens))
       (unless (null? stk)
         (set! scores (cons (fold (lambda (token total)
                         (+ (* 5 total)
                            (case token
                              ((#\)) 1)
                              ((#\]) 2)
                              ((#\}) 3)
                              ((#\>) 4))))
                                  0 (map flip stk))
                            scores))
         (format #t "Complete by adding `~a` ~a points"
                 (list->string (map flip stk))
                 (fold (lambda (token total)
                         (+ (* 5 total)
                            (case token
                              ((#\)) 1)
                              ((#\]) 2)
                              ((#\}) 3)
                              ((#\>) 4))))
                       0 (map flip stk))))
       (return #t)))
    )
  (port->stream (current-input-port) read-line)))
(newline)

(format #t "middle score = ~a~%" (list-ref (sort scores <) (/ (1- (length scores)) 2)))
