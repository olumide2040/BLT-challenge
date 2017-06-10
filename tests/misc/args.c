#include <stic.h>

#include <stdio.h> /* remove() */

#include "../../src/utils/fs.h"
#include "../../src/utils/macros.h"
#include "../../src/args.h"
#include "../../src/status.h"

#include "utils.h"

SETUP()
{
	curr_stats.load_stage = 1;
}

TEARDOWN()
{
	curr_stats.load_stage = 0;
}

TEST(chooseopt_options_are_not_set)
{
	args_t args = { };
	char *argv[] = { "vifm", "+", NULL };

	args_parse(&args, ARRAY_LEN(argv) - 1U, argv, "/");

	assert_int_equal(1, args.ncmds);
	assert_string_equal("$", args.cmds[0]);

	args_free(&args);
}

TEST(select_does_not_accept_dash_if_no_such_file)
{
	args_t args = { .lwin_handle = 135 };
	char *argv[] = { "vifm", "--select", "-", NULL };

	args_parse(&args, ARRAY_LEN(argv) - 1U, argv, "/");

	assert_string_equal("", args.lwin_path);
	/* XXX: this is a hack to postpone updating args.c unit. */
	assert_int_equal(135, args.lwin_handle);

	args_free(&args);
}

TEST(select_accepts_dash_if_such_file_exists)
{
	args_t args = { };
	char *argv[] = { "vifm", "--select", "-", NULL };

	char *const saved_cwd = save_cwd();
	create_file("-");

	args_parse(&args, ARRAY_LEN(argv) - 1U, argv, "/");

	assert_string_equal("-", args.lwin_path);

	assert_success(remove("-"));
	restore_cwd(saved_cwd);
	args_free(&args);
}

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 filetype=c : */