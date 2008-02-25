;;ajoute le chemin vers emacs dans path
(add-to-list 'load-path (expand-file-name "~/.emacs.d/"))
(add-to-list 'load-path (expand-file-name "~/.emacs.d/povray"))
(add-to-list 'load-path (expand-file-name "~/.emacs.d/kde-emacs"))
(add-to-list 'load-path (expand-file-name "~/.emacs.d/color-theme"))

;;CLisp mode
(add-to-list 'load-path "~/.emacs.d/slime/")
(setq inferior-lisp-program "/usr/bin/cmucl")
(require 'slime)
(slime-setup)


(autoload 'gtags-mode "gtags" "" t)

(require 'color-theme)
(color-theme-initialize)
(color-theme-deep-blue)

;;kde modes
(setq magic-keys-mode nil)
(require 'kde-emacs)

;;mail
(require 'smtpmail)
(setq smtpmail-smtp-server "localhost")
(setq send-mail-function 'smtpmail-send-it)
(setq user-mail-address "leserpent@berlios.de")
(setq user-full-name "Ben")

(require 'ctypes)
(savehist-mode t)
;;mode css
;(require 'css-mode)
;(autoload 'css-mode "css-mode.el" "CSS mode" t)
;(setq auto-mode-alist
;      (append '(("\\.css$" . css-mode) 
;		) auto-mode-alist))

;;Mode special QT
(load "cc-engine.elc")
;(load "kde-devel-emacs.el")
(setq auto-mode-alist
      (append '(("\\.h$" . c++-mode)
		("\\.cg$" . c++-mode)
		("\\.glsl$" . c++-mode)
		("\\.fx$" . c++-mode)) auto-mode-alist))
(setq magic-keys-mode nil)

;;Mode OpenGL
;;(autoload 'OpenGL-minor-mode "OpenGL" "OpenGL editing utilities." t)
;;(add-hook 'OpenGL-minor-mode-hook 'OpenGL-setup-keys)

;;Mode todo
(autoload 'todo-mode "todo-mode" "Major mode for editing TODO" t)
(autoload 'todo-show "todo-mode" "Show TODO items." t)
(autoload 'todo-insert-item "todo-mode" "Add TODO items" t)

;;Désactive le bip
(setq visible-bell 't)

;;Suppresion de la toolbar
(tool-bar-mode -1)

;;Gestion de la molette
(cond (window-system
       (mwheel-install)
))

;;Chargement du package de gestion des sessions
;(require 'session)
;(add-hook 'after-init-hook 'session-initialize)

;;Chargement auto du mode nasm
(autoload 'nasm-mode "nasm-mode.el" "Nasm mode" t)
(setq auto-mode-alist
      (append '(("\\.asm$" . nasm-mode) 
		("\\.inc$" . nasm-mode)
		) auto-mode-alist))

;; Set autoloading of POV-mode for these file-types.
;(require 'pov-mode)
;(autoload 'pov-mode "pov-mode.el" "PoVray scene file mode" t)
;(setq auto-mode-alist
;      (append '(("\\.pov$" . pov-mode) 
;		) auto-mode-alist))
;(add-hook 'pov-mode-hook 'turn-on-font-lock)

;Charge les fonctions de creation auto de classes
(load "ccext.el")

;;Affiche l'heure et le numero de ligne
(setq line-number-mode t)
(display-time)

;;Decompression automatique des fichiers
;;(toggle-auto-compression) ;version X
(auto-compression-mode 1)

;;Coloration automatique des buffers
;;(require 'font-lock)
;;(setq font-lock-maximum-decoration 't
;;      font-lock-background-mode 'dark)
(global-font-lock-mode t)
;;(lazy-lock-mode)
;;(setq font-lock-support-mode 'lazy-lock-mode)

;;delete efface les caracteres a l'avant 
(setq delete-key-deletes-forward 't)

;;Permet d'effacer une zone selectionnee avec delete
(cond 
 ((fboundp 'turn-on-pending-delete)
  (turn-on-pending-delete))
 ((fboundp 'pending-delete-on)
  (pendin-delete-on t)))

;;Charge desktop.el et chargement automatique du bureau
(load "desktop")
(desktop-load-default)
(desktop-read)

;;Pour sauver le bureau
(global-set-key [f1] 'desktop-save)

;;La commande untilisee par compile
(setq compile-command "make")
(global-set-key [f7] 'compile)
(global-set-key "\M-g" 'goto-line)
(global-set-key [(print)] 'font-lock-fontify-buffer)

(define-key c++-mode-map [(f7)] 'compile)

;;parentheses colorees
(require 'paren)
(show-paren-mode 1)

;;indentation en mode bash
(setq sh-basic-offset 2)

;;indentation en mode C
(defun my-c-mode-hook()
  (setq c-basic-offset 2))
(add-hook 'c-mode-hook 'my-c-mode-hook)

(setq c-basic-offset 2)
;;indentation en mode Java
(add-hook 'java-mode-hook 'my-c-mode-hook)

;;Autofill pour fichiers tex
(add-hook 'tex-mode-hook 'auto-fill-mode)

;;Fichiers d'abbrevs
;(setq save-abbrevs t)
;(setq-default abbrev-mode t)
;(read-abbrev-file "~/.abbrev_defs")

;;Aspell
(setq ispell-program-name "aspell")
(autoload 'flyspell-mode "flyspell" "On-the-fly spelling." t)
(require 'ispell)

;;Divers
(setq inhibit-startup-message t)
;(lazy-lock-mode 1)

(add-hook 'dired-load-hook (function (lambda () (load "dired-x"))))
(setq dired-omit-files-p t)
(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(ecb-options-version "2.32")
 '(gnuserv-program (concat exec-directory "/gnuserv"))
 '(load-home-init-file t t)
 '(magic-keys-mode nil)
 '(pc-select-meta-moves-sexps t)
 '(pc-select-selection-keys-only t)
 '(pc-selection-mode t nil (pc-select)))
(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 )

(load "gl.el")
(load "psvn.el")

;;Javascript
;(autoload 'javascript-mode "javascript-mode" "JavaScript mode" t)
;(setq auto-mode-alist (append '(("\\.as$" . javascript-mode))
;			      auto-mode-alist))
(require 'generic-x)
(add-to-list 'generic-extras-enable-list 'javascript-generic-mode)

;;Indente la ligne courante comme la precedente
(setq indent-line-function 'indent-relative) 
(setq indent-tabs-mode nil)


;;Options de compile
(setq compilation-scroll-output t)
(setq compilation-window-height 14)
;;Utiliser lorsque la compilation est lancée avec -C ../../
(setq compilation-search-path '("." "../.." "../../.." "../../../../"))
;(setq compile-command "scons -C .")
(global-set-key [f4] 'compile)

;;CMAKE
(setq auto-mode-alist
	  (append
	   '(("CMakeLists\\.txt\\'" . cmake-mode))
	   '(("\\.cmake\\'" . cmake-mode))
	   auto-mode-alist))
(autoload 'cmake-mode "~/.emacs.d/cmake-mode.el" t)


;;switch cxx/h
(load "cxx-tools.el")
(define-key c-mode-map [(f6)] 'cxxtools-switch-cpp-h)
(define-key c++-mode-map [(f6)] 'cxxtools-switch-cpp-h)
(define-key c-mode-map [(shift f6)] 'cxxtools-switch-to-included-file)
(define-key c++-mode-map [(shift f6)] 'cxxtools-switch-to-included-file)
