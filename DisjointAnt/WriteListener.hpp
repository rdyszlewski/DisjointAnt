class WriteListener
{
public:
	virtual void writeResult(int iteration, std::vector<int> values) = 0;
	virtual void writeLog(std::string message) = 0;

private:

};