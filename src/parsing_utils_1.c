#include "../include/msh.h"

t_exec	*create_exec()
{
	t_exec	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->type = EXEC;
	cmd->argv = NULL;
	return (cmd);
}

t_node	*diversion(t_node *left, t_node *right, int token)
{
	t_div *node;

	if (!right)
	{
		free_cmdtree(left);
		return (NULL);
	}
	node = malloc(sizeof(*node));
	if (!node)
	{
		free_cmdtree(right);
		free_cmdtree(left);
		return (NULL);
	}
	node->type = token;
	node->left = left;
	node->right = right;
	return ((t_node*)node);
}


t_node	*add_redirection(t_redirection *red, t_node *node)
{
	t_redirection	*ptr;
	t_redirection	*prev;

	red->node = node;
	if ((node->type == RED || node->type == HEREDOC) && ((t_redirection *)node)->fd == red->fd)
	{
		ptr = (t_redirection *)node;
		while ((ptr->type == RED || ptr->type == HEREDOC) && ptr->fd == red->fd)
		{
			prev = ptr;
			ptr = (t_redirection *)ptr->node;
		}
		red->node = (t_node *)ptr;
		prev->node = (t_node *)red;
		return (node);
	}
	return (t_node *)red;
}

t_node	*create_redirection(t_node *node, char *file, int flags, int fd)
{
	t_redirection	*red;

	if (!file)
		return (NULL);
	red = malloc(sizeof(*red));
	if (!red)
		return (NULL);
	ft_memset(red, 0, sizeof(*red));
	red->type = RED;
	red->file = file;
	printf("%s\n", file);
	red->flags = flags;
	red->fd = fd;
	return (add_redirection(red, node));
}

t_node	*parse_redirection_create(char *st, char *et, t_node *node, char red)
{
	char	*file;
	t_node	*tmp_node;

	file = strdup_v2(st, et);
	tmp_node = node;
	if (red == IN_RED)
		node = create_redirection(node, file, O_RDONLY, 0);
	else if (red == OUT_RED)
		node = create_redirection(node, file, O_WRONLY|O_CREAT|O_TRUNC, 1);
	else if (red == APP_RED)
		node = create_redirection(node, file, O_WRONLY|O_CREAT|O_APPEND, 1);
	if (!node)
	{
		free(file);
		free_cmdtree(tmp_node);
		return (NULL);
	}
	return (node);
}
