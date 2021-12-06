#!/usr/bin/guile -s
!#

(use-modules (srfi srfi-1)
             (rnrs records syntactic)
             (ice-9 rdelim)
             (ice-9 format)
             (ice-9 control))

(define balls (map string->number (string-split (read-line) #\,)))

(define (parse-line str)
  (map string->number (remove string-null? (string-split str #\space))))

(define boards
 (let loop ((boards '()) (current-board '()))
   (define line (read-line))
   (cond ((eof-object? line) (cdr (reverse (cons (reverse current-board) boards))))
         ((string-null? line)
          (loop (cons (reverse current-board) boards)
                '()))
         (else (loop boards
                     (cons (parse-line line) current-board))))))

(define-record-type bingo-board
  (fields (immutable row-counts)
          (immutable column-counts)
          (immutable numbers)
          (immutable source)
          (immutable marked-squares)))

(define (play-ball bingo-card ball)
  (cond ((hashv-ref (bingo-board-numbers bingo-card) ball)
         => (lambda (p)
              (define row-counts (bingo-board-row-counts bingo-card))
              (define col-counts (bingo-board-column-counts bingo-card))
              (vector-set! row-counts (car p)
                           (1+ (vector-ref row-counts (car p))))
              (vector-set! col-counts (cdr p)
                           (1+ (vector-ref col-counts (cdr p))))

              (array-set! (bingo-board-marked-squares bingo-card) #t  (cdr p)(car p))

              (or (= 5 (vector-ref row-counts (car p)))
                  (= 5 (vector-ref col-counts (cdr p))))))
        (else #f)))

(define bingo-cards
  (map (lambda (board)
         (define ht (make-hash-table))
         (for-each (lambda (y)
                     (for-each (lambda (x)
                                 (hashv-set! ht (list-ref (list-ref board y) x) (cons x y)))
                               (iota 5)))
                   (iota 5))

         (make-bingo-board (make-vector 5 0) (make-vector 5 0)
                           ht board
                           (make-array #f 5 5)))
       boards))

(define-values (board ball)
  (call/ec
   (lambda (return)
     (for-each (lambda (ball)
                 (format #t "~a~%" ball)
                 (for-each (lambda (bingo-card)
                             (when (play-ball bingo-card ball)
                               (return bingo-card ball)))
                           bingo-cards)
                 )
               balls))))

(define sum 0)

(for-each
 (lambda (y)
   (for-each
    (lambda (x)
      (define value (list-ref (list-ref (bingo-board-source board) y) x))
      (if (array-ref (bingo-board-marked-squares board) y x)
          (format #t "|\x1b[41m~2@a\x1b[m " value)
          (begin
            (set! sum (+ sum value))
            (format #t "|~2@a " value))))
    (iota 5))
   (newline))
 (iota 5))
(newline)

(format #t "~a x ~a = ~a~%"
        sum ball (* sum ball))

;; (format #t "~y" boards)
;; (format #t "~y" balls)
