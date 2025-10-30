# DiSH is a simple command interpreter

## TODO
- [ ] Lexer quotes mode
- [ ] Parser
- [ ] Executor

## Lexer
### Token table
| Token | Lexeme |
| --- | --- |
| BACKGROUND | & |
| SEQUENCE | ; |
| INPUT_REDIRECTION | < |
| OUTPUT_REDIRECTION | > |
| APPEND_REDIRECTION | >> |
| PIPE | \| |
| LBRACKET | ( |
| RBRACKET | ) |
| AND | && |
| OR | \|\| |
| WORD | everything else |
