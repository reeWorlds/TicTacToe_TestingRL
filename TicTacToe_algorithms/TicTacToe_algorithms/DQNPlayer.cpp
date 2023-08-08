#include "DQNPlayer.h"

void DQNPlayer::NN::fillZeroForward()
{
	for (auto& it : input) { it = 0.0f; }
	for (auto& it : l1) { it = 0.0f; }
	for (auto& it : l2) { it = 0.0f; }
	for (auto& it : output) { it = 0.0f; }
}

void DQNPlayer::NN::fillZeroBackward()
{
	fillZeroForward();

	for (auto& it : l1_d) { it = 0.0f; }
	for (auto& it : l2_d) { it = 0.0f; }
	output_d = 0.0f;

	for (auto& it : l1_dw) { for (auto& itt : it) { itt = 0.0f; } }
	for (auto& it : l2_dw) { for (auto& itt : it) { itt = 0.0f; } }
	for (auto& it : output_dw) { it = 0.0f; }

	for (auto& it : l1_db) { it = 0.0f; }
	for (auto& it : l2_db) { it = 0.0f; }
}

float DQNPlayer::NN::relu(float x)
{
	return x < 0.0f ? 0.1f : (x < 1.0 ? x : 1.0 + (x - 1.0) * 0.1f);
}

float DQNPlayer::NN::relu_prime(float y)
{
	return y < 0.0f ? 0.1f : (y < 1.0 ? 1.0 : 0.1f);
}

void DQNPlayer::NN::readWeights()
{
	ifstream in("saves/DQNPlayer.txt", ios::binary);

	in.read((char*)l1_w, sizeof(l1_w));
	in.read((char*)l2_w, sizeof(l2_w));
	in.read((char*)output_w, sizeof(output_w));

	in.read((char*)l1_b, sizeof(l1_b));
	in.read((char*)l2_b, sizeof(l2_b));

	in.close();
}

void DQNPlayer::NN::writeWeights()
{
	ofstream out("saves/DQNPlayer.txt", ios::binary);

	out.write((char*)l1_w, sizeof(l1_w));
	out.write((char*)l2_w, sizeof(l2_w));
	out.write((char*)output_w, sizeof(output_w));

	out.write((char*)l1_b, sizeof(l1_b));
	out.write((char*)l2_b, sizeof(l2_b));

	out.close();
}

void DQNPlayer::NN::initWeights(mt19937& rng)
{
	normal_distribution<float> dist(0.0f, 1.0f);

	for (auto& it : l1_w) { for (auto& itt : it) { itt = dist(rng) / sqrt(18.0); } }
	for (auto& it : l2_w) { for (auto& itt : it) { itt = dist(rng) / sqrt(16.0); } }
	for (auto& it : output_w) { for (auto& itt : it) { itt = dist(rng) / sqrt(8.0); } }
	for (auto& it : l1_b) { it = dist(rng); }
	for (auto& it : l2_b) { it = dist(rng); }
}

void DQNPlayer::NN::forward(Matrix& matrix, float Qs[9])
{
	fillZeroForward();

	for (int i = 0; i < 9; i++)
	{
		if (matrix.a[i / 3][i % 3] == 1)
		{
			input[i] = 1.0f;
		}
		else if (matrix.a[i / 3][i % 3] == -1)
		{
			input[i + 9] = 1.0f;
		}
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			l1[i] += l1_w[i][j] * input[j];
		}
		l1[i] += l1_b[i];
		l1[i] = relu(l1[i]);
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			l2[i] += l2_w[i][j] * l1[j];
		}
		l2[i] += l2_b[i];
		l2[i] = relu(l2[i]);
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			output[i] += output_w[i][j] * l2[j];
		}
		Qs[i] = output[i];
	}
}

void DQNPlayer::NN::forwardBackward(Matrix& matrix, int actionI, float Qi_gt, float lr)
{
	// reinitialize data from fowrward pass
	forward(matrix, output);

	// backward pass
	output_d = output[actionI] - Qi_gt;
	for (int j = 0; j < 8; j++)
	{
		output_dw[j] = output_d * l2[j];
	}

	for (int i = 0; i < 8; i++)
	{
		l2_d[i] = output_d * output_w[actionI][i] * relu_prime(l2[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			l2_dw[i][j] = l2_d[i] * l1[j];
		}
		l2_db[i] = l2_d[i];
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			l1_d[i] += l2_d[j] * l2_w[j][i];
		}
		l1_d[i] *= relu_prime(l1[i]);
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			l1_dw[i][j] = l1_d[i] * input[j];
		}
		l1_db[i] = l1_d[i];
	}

	// update weights and biases
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			output_w[i][j] -= lr * output_dw[j];
		}
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			l2_w[i][j] -= lr * l2_dw[i][j];
		}
		l2_b[i] -= lr * l2_db[i];
	}

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			l1_w[i][j] -= lr * l1_dw[i][j];
		}
		l1_b[i] -= lr * l1_db[i];
	}
}

DQNPlayer::DQNPlayer() : rng(chrono::high_resolution_clock::now().time_since_epoch().count())
{
	nn = nullptr;
}

DQNPlayer::~DQNPlayer()
{
	if (nn != nullptr)
	{
		delete[] nn;
	}
}

void DQNPlayer::playAndUpdateNN(float epsilon1, float epsilon2, float alpha)
{
	vector <int> history; // xy

	Matrix matrix;
	int side = 0;

	vector <int> options;

	float Qs[9];
	vector <pair<float, int> > prbs; // {value, xy}

	while (!matrix.isFinished())
	{
		int hash = matrix.getHash();

		float epsilon_s = (rng() % 1000000) / 1000000.0;

		int val = side == 0 ? 1 : -1;

		if (epsilon_s < epsilon1)
		{
			// choose random move
			options.clear();

			for (int xy = 0; xy < 9; xy++)
			{
				if (matrix.a[xy / 3][xy % 3] == 0) { options.push_back(xy); }
			}

			int xy = options[rng() % options.size()];

			matrix.a[xy / 3][xy % 3] = val;
			history.push_back(xy);
		}
		else if (epsilon_s < epsilon2)
		{
			// pseudo-softmax
			prbs.clear();

			nn->forward(matrix, Qs);
			for (int xy = 0; xy < 9; xy++)
			{
				if (matrix.a[xy / 3][xy % 3] == 0) { prbs.push_back({ Qs[xy], xy }); }
			}

			if (side == 1)
			{
				for (auto& it : prbs)
				{
					it.first *= -1.0;
				}
			}

			applySoftmax(prbs);

			float r = (rng() % 100000) / 100000.0;

			int xy = 0;
			while (r > prbs[xy].first)
			{
				r -= prbs[xy].first;
				xy++;
			}

			xy = prbs[xy].second;

			matrix.a[xy / 3][xy % 3] = val;
			history.push_back(xy);
		}
		else
		{
			// choose move greedily
			float bestScore = val * -1e9;
			int bestXY;

			nn->forward(matrix, Qs);

			for (int xy = 0; xy < 9; xy++)
			{
				if (matrix.a[xy / 3][xy % 3] == 0)
				{
					float score = Qs[xy];

					if (side == 0)
					{
						if (score > bestScore)
						{
							bestScore = score;
							bestXY = xy;
						}
					}
					else
					{
						if (score < bestScore)
						{
							bestScore = score;
							bestXY = xy;
						}
					}
				}
			}

			matrix.a[bestXY / 3][bestXY % 3] = val;
			history.push_back(bestXY);
		}

		side ^= 1;
	}

	float reward = matrix.checkState();

	matrix.initZero();

	for (int i = 0; i < history.size(); i++)
	{
		int side = i % 2, xy = history[i];
		float val = side == 0 ? 1 : -1;

		nn->forwardBackward(matrix, xy, reward, alpha);

		matrix.a[xy / 3][xy % 3] = val;
	}
}

void DQNPlayer::trainNN()
{
	nn->initWeights(rng);

	const int N_Games = 10000;

	for (int gameI = 0; gameI < N_Games; gameI++)
	{
		float epsilon1 = pow(1.0 - double(gameI) / N_Games, 2.5) * 0.9;
		float epsilon2 = 0.2 + 0.8 * pow(1.0 - double(gameI) / N_Games, 1.5);
		float alpha = 0.1 - 0.09 * gameI / N_Games;

		playAndUpdateNN(epsilon1, epsilon2, alpha);
	}
}

void DQNPlayer::applySoftmax(vector<pair<float, int> >& prbs)
{
	float mx = prbs[0].first;
	for (auto& it : prbs) { mx = max(mx, it.first); }

	for (auto& it : prbs)
	{
		it.first = exp(it.first - mx);
	}

	float sum = 0.0;
	for (auto& it : prbs) { sum += it.first; }

	for (auto& it : prbs)
	{
		it.first /= sum;
	}
}

void DQNPlayer::train()
{
	nn = new NN;

	if (filesystem::exists("saves/DQNPlayer.txt"))
	{
		nn->readWeights();
	}
	else
	{
		trainNN();
		nn->writeWeights();
	}
}

void DQNPlayer::makeMove(Matrix& matrix, int side)
{
	float Qs[9];

	nn->forward(matrix, Qs);

	int val = side == 0 ? 1 : -1;

	float bestScore = val * -1e9;
	int bestXY;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (matrix.a[i][j] == 0)
			{
				float score = Qs[i * 3 + j];

				if (side == 0)
				{
					if (score > bestScore)
					{
						bestScore = score;
						bestXY = i * 3 + j;
					}
				}
				else
				{
					if (score < bestScore)
					{
						bestScore = score;
						bestXY = i * 3 + j;
					}
				}
			}
		}
	}

	matrix.a[bestXY / 3][bestXY % 3] = val;
}