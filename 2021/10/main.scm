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

(format #t "~a~%"
        (apply +
         (map lookup-points
              (stream->list
               (stream-map
                (lambda (line)
                  (define tokens (string->list line))
                  (call/ec
                   (lambda (return)
                     (fold (lambda (token stack)
                             (if (memv token '(#\{ #\( #\[ #\<))
                                 (cons token stack)
                                 ;; else closing token
                                 (if (null? stack)
                                     ;; input incomplete
                                     (begin
                                       (format #t "Unexpected end of input~%")
                                       (return #f))
                                     (if (char=? (flip token)
                                                 (car stack))
                                         ;; correct ending token, pop it off the stack
                                         (cdr stack)
                                         ;; incorrect ending
                                         (begin
                                           (format #t "Expected `~a`, but found `~a` instead.~%"
                                                   (flip (car stack)) token)
                                           (return token))))))
                           '() tokens)
                     (return #t)))
                  )
                (port->stream (current-input-port) read-line))))))
