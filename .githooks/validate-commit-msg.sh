#!/bin/bash
# .githooks/validate-commit-msg.sh

commit_msg_file=$1
commit_msg=$(cat "$commit_msg_file")

pattern="^(feat|fix|docs|style|refactor|perf|test|build|ci|chore)(\(.+\))?: .{1,50}"

if ! echo "$commit_msg" | grep -qE "$pattern"; then
    cat << EOF
Invalid commit format!

Expected format: <type>(<scope>): <description>

Allowed types: 

    feat: Adding new functionality
    fix: Squashing bugs
    refactor: Improving code without changing behavior
    docs: Documentation updates
    style: Formatting and code style fixes
    test: Adding or updating tests
    chore: Maintenance tasks like dependency updates
    perf: Performance optimizations
    ci: Continuous integration tweaks

Examples:

    feat(auth): add user login endpoint
    fix(dashboard): correct chart rendering issue
    docs(readme): update project setup instructions
    refactor(utils): extract date formatter function
    chore: update dependencies to latest versions

Your message: "$commit_msg"
EOF
    exit 1
fi

exit 0