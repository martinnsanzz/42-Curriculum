int	ft_toupper(int c)
{
	c = (unsigned char)c;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
