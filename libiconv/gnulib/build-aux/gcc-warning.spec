# options to filter out, and why
--all-warnings				alias for -Wall
--extra-warnings			alias for -Wextra
-W					alias for -Wextra
-Wabi					this is now a no-op
-Wabi-tag				c++
-Wabi=					c++
-Wabsolute-value			enabled by -Wextra
-Waddress				enabled by -Wall
-Waddress-of-packed-member		default
-Waggregate-return			obsolescent
-Waggressive-loop-optimizations		default
-Waliasing				fortran
-Walign-commons				fortran
-Waligned-new=[none|global|all]		c++
-Walloc-size-larger-than=<bytes>	defaults to PTRDIFF_MAX
-Walloc-zero				Gnulib fixes this problem
-Walloca				we like alloca in small doses
-Walloca-larger-than=<number>		FIXME: choose something sane?
-Wampersand				fortran
-Wanalyzer-allocation-size		enabled by -fanalyzer
-Wanalyzer-deref-before-check		enabled by -fanalyzer
-Wanalyzer-double-fclose		enabled by -fanalyzer
-Wanalyzer-double-free			enabled by -fanalyzer
-Wanalyzer-exposure-through-output-file	enabled by -fanalyzer
-Wanalyzer-exposure-through-uninit-copy	enabled by -fanalyzer
-Wanalyzer-fd-access-mode-mismatch	enabled by -fanalyzer
-Wanalyzer-fd-double-close		enabled by -fanalyzer
-Wanalyzer-fd-leak			enabled by -fanalyzer
-Wanalyzer-fd-phase-mismatch		enabled by -fanalyzer
-Wanalyzer-fd-type-mismatch		enabled by -fanalyzer
-Wanalyzer-fd-use-after-close		enabled by -fanalyzer
-Wanalyzer-fd-use-without-check		enabled by -fanalyzer
-Wanalyzer-file-leak			enabled by -fanalyzer
-Wanalyzer-free-of-non-heap		enabled by -fanalyzer
-Wanalyzer-imprecise-fp-arithmetic	enabled by -fanalyzer
-Wanalyzer-infinite-recursion		enabled by -fanalyzer
-Wanalyzer-jump-through-null		enabled by -fanalyzer
-Wanalyzer-malloc-leak			enabled by -fanalyzer
-Wanalyzer-mismatching-deallocation	enabled by -fanalyzer
-Wanalyzer-null-argument		enabled by -fanalyzer
-Wanalyzer-null-dereference		enabled by -fanalyzer
-Wanalyzer-out-of-bounds		enabled by -fanalyzer
-Wanalyzer-possible-null-argument	enabled by -fanalyzer
-Wanalyzer-possible-null-dereference	enabled by -fanalyzer
-Wanalyzer-putenv-of-auto-var		enabled by -fanalyzer
-Wanalyzer-shift-count-negative		enabled by -fanalyzer
-Wanalyzer-shift-count-overflow		enabled by -fanalyzer
-Wanalyzer-stale-setjmp-buffer		implied by -fanalyzer
-Wanalyzer-tainted-allocation-size	FIXME requires -fanalyzer-checker=taint
-Wanalyzer-tainted-array-index		FIXME requires -fanalyzer-checker=taint
-Wanalyzer-tainted-assertion		FIXME requires -fanalyzer-checker=taint
-Wanalyzer-tainted-divisor		FIXME requires -fanalyzer-checker=taint
-Wanalyzer-tainted-offset		FIXME requires -fanalyzer-checker=taint
-Wanalyzer-tainted-size			FIXME requires -fanalyzer-checker=taint
-Wanalyzer-va-arg-type-mismatch		enabled by -fanalyzer
-Wanalyzer-va-list-exhausted		enabled by -fanalyzer
-Wanalyzer-va-list-leak			enabled by -fanalyzer
-Wanalyzer-va-list-use-after-va-end	enabled by -fanalyzer
-Wanalyzer-too-complex			enabled by -fanalyzer
-Wanalyzer-unsafe-call-within-signal-handler	enabled by -fanalyzer
-Wanalyzer-use-after-free		enabled by -fanalyzer
-Wanalyzer-use-of-pointer-in-stale-stack-frame	enabled by -fanalyzer
-Wanalyzer-use-of-uninitialized-value	enabled by -fanalyzer
-Wanalyzer-write-to-const		enabled by -fanalyzer
-Wanalyzer-write-to-string-literal	enabled by -fanalyzer
-Warray-bounds				covered by -Warray-bounds=
-Warray-bounds=<0,2>			handled specially by gl_MANYWARN_ALL_GCC
-Warray-compare				enabled by -Wall
-Warray-parameter			enabled by -Wall
-Warray-parameter=<0,2>			enabled by -Wall
-Warray-temporaries			fortran
-Wassign-intercept			objc/objc++
-Wattribute-alias			covered by -Wattribute-alias=2
-Wattribute-alias=<0,2>			handled specially by gl_MANYWARN_ALL_GCC
-Wattribute-warning			default
-Wattributes				default
-Wbidi-chars				handled specially by gl_MANYWARN_ALL_GCC
-Wbidi-chars=				handled specially by gl_MANYWARN_ALL_GCC
-Wbool-compare				enabled by -Wall
-Wbool-operation			enabled by -Wall
-Wbuiltin-declaration-mismatch		default
-Wbuiltin-macro-redefined		default
-Wc++-compat				only useful for code meant to be compiled by a C++ compiler
-Wc++0x-compat				c++
-Wc++11-compat				c++
-Wc++11-extensions			c++
-Wc++14-compat				c++
-Wc++14-extensions			c++
-Wc++17-compat				c++
-Wc++17-extensions			c++
-Wc++1z-compat				c++
-Wc++20-compat				c++
-Wc++20-extensions			c++
-Wc++23-extensions			c++
-Wc++2a-compat				c++
-Wc-binding-type			fortran
-Wc11-c2x-compat			c compatibility
-Wc90-c99-compat			c compatibility
-Wc99-c11-compat			c compatibility
-Wcannot-profile			default
-Wcast-align				enabled by -Wcast-align=strict
-Wcast-function-type			enabled by -Wextra
-Wcast-qual				FIXME maybe? too much noise; encourages bad changes
-Wcast-result				D
-Wcatch-value				c++
-Wcatch-value=<0,3>			c++
-Wchanges-meaning			c++
-Wchar-subscripts			enabled by -Wall
-Wcharacter-truncation			fortran
-Wchkp					deprecated
-Wclass-conversion			c++ and objc++
-Wclass-memaccess			c++
-Wclobbered				enabled by -Wextra
-Wcomma-subscript			c++ and objc++
-Wcomment				enabled by -Wall
-Wcomments				alias for -Wcomment
-Wcompare-reals				fortran
-Wcomplain-wrong-lang			default
-Wconditionally-supported		c++ and objc++
-Wconversion				FIXME maybe? too much noise; encourages bad changes
-Wconversion-extra			fortran
-Wconversion-null			c++ and objc++
-Wcoverage-invalid-line-number		default if --coverage
-Wcoverage-mismatch			default
-Wcpp					default
-Wctad-maybe-unsupported		c++ and objc++
-Wctor-dtor-privacy			c++
-Wdangling-else				enabled by -Wparentheses
-Wdangling-pointer			enabled by -Wall
-Wdangling-pointer=<0,2>		enabled by -Wall
-Wdangling-reference			c++ and objc++
-Wdeclaration-after-statement		needed only for pre-C99, so obsolete
-Wdelete-incomplete			c++ and objc++
-Wdelete-non-virtual-dtor		c++
-Wdeprecated				default
-Wdeprecated-copy			c++ and objc++
-Wdeprecated-copy-dtor			c++ and objc++
-Wdeprecated-declarations		default
-Wdeprecated-enum-enum-conversion	c++ and objc++
-Wdeprecated-enum-float-conversion	c++ and objc++
-Wdesignated-init			default
-Wdiscarded-array-qualifiers		default
-Wdiscarded-qualifiers			default
-Wdo-subscript				fortran
-Wdiv-by-zero				default
-Wduplicate-decl-specifier		enabled by -Wall
-Weffc++				c++
-Wempty-body				enabled by -Wextra
-Wenum-compare				enabled by -Wall
-Wenum-conversion			enabled by -Wextra
-Wenum-int-mismatch			enabled by -Wall
-Wendif-labels				default
-Werror-implicit-function-declaration	deprecated
-Wexceptions				c++ and objc++
-Wexpansion-to-defined			enabled by -Wextra
-Wextra-semi				c++
-Wfloat-conversion			FIXME maybe? borderline.  some will want this
-Wfloat-equal				FIXME maybe? borderline.  some will want this
-Wformat				covered by -Wformat=2
-Wformat-contains-nul			default
-Wformat-diag				enabled by -Wformat=2
-Wformat-extra-args			enabled by -Wformat=2
-Wformat-nonliteral			enabled by -Wformat=2
-Wformat-overflow<0,2>			gcc --help=warnings artifact
-Wformat-overflow=<0,2>			handled specially by gl_MANYWARN_ALL_GCC
-Wformat-security			enabled by -Wformat=2
-Wformat-truncation			covered by -Wformat-truncation=2
-Wformat-truncation=<0,2>		handled specially by gl_MANYWARN_ALL_GCC
-Wformat-y2k				enabled by -Wformat=2
-Wformat-zero-length			enabled by -Wformat=2
-Wformat=<0,2>				gcc --help=warnings artifact
-Wframe-address				enabled by -Wall
-Wframe-larger-than=<byte-size>		FIXME: choose something sane?
-Wfree-nonheap-object			default
-Wfunction-elimination			fortran
-Whsa					default
-Wif-not-aligned			default
-Wignored-attributes			default
-Wignored-qualifiers			enabled by -Wextra
-Wimplicit				enabled by -Wall
-Wimplicit-fallthrough			covered by -Wimplicit-fallthrough=2
-Wimplicit-fallthrough=<0,5>		handled specially by gl_MANYWARN_ALL_GCC
-Wimplicit-function-declaration		enabled by -Wimplicit
-Wimplicit-int				enabled by -Wimplicit
-Wimplicit-interface			fortran
-Wimplicit-procedure			fortran
-Winaccessible-base			c++ and objc++
-Wincompatible-pointer-types		default
-Winfinite-recursion			enabled by -Wall
-Winherited-variadic-ctor		c++
-Winit-list-lifetime			c++ and objc++
-Wint-conversion			default
-Wint-in-bool-context			enabled by -Wall
-Wint-to-pointer-cast			default
-Winteger-division			fortran
-Winterference-size			c++
-Wintrinsic-shadow			fortran
-Wintrinsics-std			fortran
-Winvalid-constexpr			c++
-Winvalid-imported-macros		c++ and objc++
-Winvalid-memory-model			default
-Winvalid-offsetof			c++ and objc++
-Winvalid-utf8				enabled by -finput-charset=UTF-8
-Wjump-misses-init			only useful for code meant to be compiled by a C++ compiler
-Wlarger-than-				gcc --help=warnings artifact
-Wlarger-than=<byte-size>		FIXME: choose something sane?
-Wline-truncation			fortran
-Wliteral-suffix			c++ and objc++
-Wlogical-not-parentheses		enabled by -Wall
-Wlong-long				obsolescent
-Wlto-type-mismatch			c++ and objc++
-Wmain					enabled by -Wall
-Wmaybe-uninitialized			enabled by -Wall or -Wextra
-Wmemset-elt-size			enabled by -Wall
-Wmemset-transposed-args		enabled by -Wall
-Wmisleading-indentation		enabled by -Wall
-Wmismatched-dealloc			default
-Wmismatched-new-delete			default, c++ and objc++
-Wmismatched-tags			c++ and objc++
-Wmissing-attributes			enabled by -Wall
-Wmissing-braces			enabled by -Wall
-Wmissing-field-initializers		enabled by -Wextra
-Wmissing-format-attribute		obsolescent
-Wmissing-noreturn			obsolescent
-Wmissing-parameter-type		enabled by -Wextra
-Wmissing-profile			default
-Wmissing-requires			default, c++
-Wmissing-template-keyword		default, c++
-Wmultichar				default
-Wmultiple-inheritance			c++ and objc++
-Wmultistatement-macros			enabled by -Wall
-Wnamespaces				c++
-Wnarrowing				enabled by -Wall
-Wno-alloc-size-larger-than		see -Walloc-size-larger-than
-Wno-alloca-larger-than			see -Walloca-larger-than
-Wno-frame-larger-than			see -Wframe-larger-than
-Wno-larger-than			see -Wlarger-than
-Wno-stack-usage			see -Wstack-usage
-Wno-vla-larger-than			see -Wvla-larger-than
-Wnoexcept				c++
-Wnoexcept-type				c++
-Wnon-template-friend			c++
-Wnon-virtual-dtor			c++
-Wnonnull				enabled by -Wall or -Wformat
-Wnonnull-compare			enabled by -Wall
-Wnormalized				default
-Wnormalized=[none|id|nfc|nfkc]		defaults to nfc
-WNSObject-attribute			objc and objc++
-Wobjc-root-class			objc and objc++
-Wodr					default
-Wold-style-cast			c++ and objc++
-Wold-style-declaration			enabled by -Wextra
-Wopenacc-parallelism			OpenACC
-Woverflow				default
-Woverloaded-virtual			c++
-Woverloaded-virtual=<0,2>		c++
-Woverride-init				enabled by -Wextra
-Woverride-init-side-effects		c++ and objc++
-Woverwrite-recursive			fortran
-Wpacked-bitfield-compat		default
-Wpacked-not-aligned			enabled by -Wall
-Wpadded				FIXME maybe?  warns about "stabil" member in /usr/include/bits/timex.h
-Wparentheses				enabled by -Wall
-Wpedantic				FIXME: too strict?
-Wpessimizing-move			c++ and objc++
-Wplacement-new				c++
-Wplacement-new=<0,2>			c++
-Wpmf-conversions			c++ and objc++
-Wpointer-compare			default
-Wpointer-sign				enabled by -Wall
-Wpointer-to-int-cast			default
-Wpragmas				default
-Wprio-ctor-dtor			c++
-Wproperty-assign-default		objc++
-Wprotocol				objc++
-Wpsabi					default
-Wrange-loop-construct			c++ and objc++
-Wreal-q-constant			fortran
-Wrealloc-lhs				fortran
-Wrealloc-lhs-all			fortran
-Wredundant-decls			FIXME maybe? many _gl_cxxalias_dummy FPs
-Wredundant-move			c++ and objc++
-Wredundant-tags			c++ and objc++
-Wregister				c++ and objc++
-Wreorder				c++ and objc++
-Wrestrict				enabled by -Wall
-Wreturn-local-addr			default
-Wreturn-type				enabled by -Wall
-Wscalar-storage-order			default
-Wselector				objc and objc++
-Wsequence-point			enabled by -Wall
-Wself-move				c++ and objc++
-Wshadow-compatible-local		covered by -Wshadow
-Wshadow-ivar				objc
-Wshadow-local				covered by -Wshadow
-Wshadow=compatible-local		covered by -Wshadow
-Wshadow=global				covered by -Wshadow
-Wshadow=local				covered by -Wshadow
-Wshift-count-negative			default
-Wshift-count-overflow			default
-Wshift-negative-value			enabled by -Wextra
-Wshift-overflow			covered by -Wshift-overflow=2
-Wshift-overflow=<0,2>			gcc --help=warnings artifact
-Wsign-compare				enabled by -Wextra
-Wsign-conversion			FIXME maybe? borderline.  some will want this
-Wsign-promo				c++ and objc++
-Wsized-deallocation			c++ and objc++
-Wsizeof-array-argument			default
-Wsizeof-array-div			enabled by -Wall
-Wsizeof-pointer-div			enabled by -Wall
-Wsizeof-pointer-memaccess		enabled by -Wall
-Wstack-usage=<byte-size>		FIXME: choose something sane?
-Wstrict-aliasing			enabled by -Wall
-Wstrict-aliasing=<0,3>			FIXME: choose something sane?
-Wstrict-null-sentinel			c++ and objc++
-Wstrict-overflow=<0,5>			FIXME: choose something sane?
-Wstrict-selector-match			objc and objc++
-Wstring-compare			enabled by -Wextra
-Wstringop-overflow			covered by -Wstringop-overflow=2
-Wstringop-overflow=<0,4>		defaults to 2
-Wstringop-overread			default
-Wstringop-truncation			default
-Wsubobject-linkage			c++ and objc++
-Wsuggest-override			c++ and objc++
-Wsurprising				fortran
-Wswitch				enabled by -Wall
-Wswitch-bool				default
-Wswitch-default			https://lists.gnu.org/r/bug-gnulib/2018-05/msg00179.html
-Wswitch-enum				FIXME maybe? borderline.  some will want this
-Wswitch-outside-range			default
-Wswitch-unreachable			default
-Wsynth					deprecated
-Wtabs					fortran
-Wtarget-lifetime			fortran
-Wtautological-compare			enabled by -Wall
-Wtemplates				c++ and objc++
-Wterminate				c++ and objc++
-Wtraditional				obsolescent
-Wtraditional-conversion		obsolescent
-Wtrigraphs				enabled by -Wall
-Wtrivial-auto-var-init			subsumed by -Wmaybe-uninitialized
-Wtsan					default
-Wtype-limits				enabled by -Wextra
-Wundeclared-selector			objc and objc++
-Wundef					FIXME maybe? too many false positives
-Wundefined-do-loop			fortran
-Wunderflow				fortran
-Wunicode				default
-Wunreachable-code			obsolescent no-op
-Wunsuffixed-float-constants		triggers warning in gnulib's timespec.h
-Wunused				enabled by -Wall
-Wunused-but-set-parameter		enabled by -Wunused
-Wunused-but-set-variable		enabled by -Wunused
-Wunused-const-variable			covered by -Wunused-const-variable=2
-Wunused-const-variable=<0,2>		gcc --help=warnings artifact
-Wunused-dummy-argument			fortran
-Wunused-function			enabled by -Wunused
-Wunused-label				enabled by -Wunused
-Wunused-local-typedefs			enabled by -Wunused
-Wunused-parameter			enabled by -Wunused
-Wunused-result				enabled by -Wunused
-Wunused-value				enabled by -Wunused
-Wunused-variable			enabled by -Wunused
-Wuse-after-free			handled specially by gl_MANYWARN_ALL_GCC
-Wuse-after-free=<0,3>			handled specially by gl_MANYWARN_ALL_GCC
-Wuse-without-only			fortran
-Wuseless-cast				c++ and objc++
-Wvarargs				default
-Wvexing-parse				c++ and objc++
-Wvirtual-inheritance			c++
-Wvirtual-move-assign			c++
-Wvla-larger-than=<number>		handled specially by gl_MANYWARN_ALL_GCC
-Wvla-parameter				enabled by -Wall
-Wvolatile				c++ and objc++
-Wvolatile-register-var			enabled by -Wall
-Wxor-used-as-pow			default
-Wzero-as-null-pointer-constant		c++ and objc++
-Wzero-length-bounds			enabled by -Wall
-Wzerotrip				fortran
-frequire-return-statement		go