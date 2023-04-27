//
// Created by fss on 23-4-27.
//
#include <benchmark/benchmark.h>
#include "../source/layer/details/expression.hpp"
#include "../source/layer/details/hardswish.hpp"
#include "../source/layer/details/linear.hpp"
#include "../source/layer/details/maxpooling.hpp"
#include "../source/layer/details/sigmoid.hpp"
#include "../source/layer/details/silu.hpp"
#include "../source/layer/details/view.hpp"

static void BM_Sigmoid(benchmark::State& state) {
  using namespace kuiper_infer;

  uint32_t channels = state.range(0);
  uint32_t rows = state.range(1);
  uint32_t cols = state.range(2);

  sftensor input = std::make_shared<ftensor>(channels, rows, cols);
  input->Fill(1.f);

  std::vector<sftensor> outputs(1);
  std::vector<sftensor> inputs;
  inputs.push_back(input);

  SigmoidLayer sigmoid_layer;
  for (auto _ : state) {
    sigmoid_layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_Sigmoid)->Args({3, 320, 320});
BENCHMARK(BM_Sigmoid)->Args({32, 160, 160});
BENCHMARK(BM_Sigmoid)->Args({64, 80, 80});
BENCHMARK(BM_Sigmoid)->Args({128, 40, 40});

static void BM_Linear(benchmark::State& state) {
  using namespace kuiper_infer;
  const int32_t in_features = (int32_t)state.range(0);
  const int32_t out_features = (int32_t)state.range(1);
  const int32_t in_dims = (int32_t)state.range(2);

  LinearLayer linear_layer(in_features, out_features, false);
  std::vector<float> weights(in_features * out_features, 1.f);
  linear_layer.set_weights(weights);

  std::shared_ptr<Tensor<float>> input =
      std::make_shared<Tensor<float>>(1, in_dims, in_features);
  input->Fill(1.f);

  std::shared_ptr<Tensor<float>> output =
      std::make_shared<Tensor<float>>(1, in_dims, out_features);
  std::vector<std::shared_ptr<Tensor<float>>> inputs;
  inputs.push_back(input);

  std::vector<std::shared_ptr<Tensor<float>>> outputs;
  outputs.push_back(output);

  for (auto _ : state) {
    linear_layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_Linear)->Args({3, 32, 1000});
BENCHMARK(BM_Linear)->Args({32, 64, 1000});
BENCHMARK(BM_Linear)->Args({64, 128, 1000});
BENCHMARK(BM_Linear)->Args({128, 512, 1000});
BENCHMARK(BM_Linear)->Args({512, 1024, 1000});

static void BM_Expression(benchmark::State& state) {
  const int32_t channels = (int32_t)state.range(0);
  const int32_t rows = (int32_t)state.range(1);
  const int32_t cols = (int32_t)state.range(2);

  using namespace kuiper_infer;
  const std::string& str = "mul(add(@0,@1),add(@2,@3))";
  ExpressionLayer layer(str);
  std::shared_ptr<Tensor<float>> input1 =
      std::make_shared<Tensor<float>>(channels, rows, cols);
  input1->Fill(2.f);
  std::shared_ptr<Tensor<float>> input2 =
      std::make_shared<Tensor<float>>(channels, rows, cols);
  input2->Fill(3.f);

  std::shared_ptr<Tensor<float>> input3 =
      std::make_shared<Tensor<float>>(channels, rows, cols);
  input3->Fill(4.f);

  std::shared_ptr<Tensor<float>> input4 =
      std::make_shared<Tensor<float>>(channels, rows, cols);
  input4->Fill(4.f);

  std::vector<std::shared_ptr<Tensor<float>>> inputs;
  inputs.push_back(input1);
  inputs.push_back(input2);
  inputs.push_back(input3);
  inputs.push_back(input4);

  std::vector<std::shared_ptr<Tensor<float>>> outputs(1);
  for (auto _ : state) {
    layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_Expression)->Args({3, 320, 320});
BENCHMARK(BM_Expression)->Args({32, 160, 160});
BENCHMARK(BM_Expression)->Args({64, 80, 80});
BENCHMARK(BM_Expression)->Args({128, 40, 40});

static void BM_HardSwish(benchmark::State& state) {
  using namespace kuiper_infer;

  uint32_t channels = state.range(0);
  uint32_t rows = state.range(1);
  uint32_t cols = state.range(2);

  sftensor input = std::make_shared<ftensor>(channels, rows, cols);
  input->Fill(1.f);

  std::vector<sftensor> outputs(1);
  std::vector<sftensor> inputs;
  inputs.push_back(input);

  HardSwishLayer hardswish_layer;
  for (auto _ : state) {
    hardswish_layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_HardSwish)->Args({3, 320, 320});
BENCHMARK(BM_HardSwish)->Args({32, 160, 160});
BENCHMARK(BM_HardSwish)->Args({64, 80, 80});
BENCHMARK(BM_HardSwish)->Args({128, 40, 40});

static void BM_SiLU(benchmark::State& state) {
  using namespace kuiper_infer;

  uint32_t channels = state.range(0);
  uint32_t rows = state.range(1);
  uint32_t cols = state.range(2);

  sftensor input = std::make_shared<ftensor>(channels, rows, cols);
  input->Fill(1.f);

  std::vector<sftensor> outputs(1);
  std::vector<sftensor> inputs;
  inputs.push_back(input);

  SiLULayer silu_layer;
  for (auto _ : state) {
    silu_layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_SiLU)->Args({3, 320, 320});
BENCHMARK(BM_SiLU)->Args({32, 160, 160});
BENCHMARK(BM_SiLU)->Args({64, 80, 80});
BENCHMARK(BM_SiLU)->Args({128, 40, 40});

static void BM_MaxPooling_k3x3s1x1(benchmark::State& state) {
  using namespace kuiper_infer;

  uint32_t channels = state.range(0);
  uint32_t rows = state.range(1);
  uint32_t cols = state.range(2);

  sftensor input = std::make_shared<ftensor>(channels, rows, cols);
  input->Fill(1.f);

  std::vector<sftensor> outputs(1);
  std::vector<sftensor> inputs;
  inputs.push_back(input);

  const uint32_t kernel_h = 3;
  const uint32_t kernel_w = 3;
  const uint32_t stride_h = 3;
  const uint32_t stride_w = 3;

  MaxPoolingLayer max_layer(0, 0, kernel_h, kernel_w, stride_h, stride_w);
  for (auto _ : state) {
    max_layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_MaxPooling_k3x3s1x1)->Args({3, 320, 320});
BENCHMARK(BM_MaxPooling_k3x3s1x1)->Args({32, 160, 160});
BENCHMARK(BM_MaxPooling_k3x3s1x1)->Args({64, 80, 80});
BENCHMARK(BM_MaxPooling_k3x3s1x1)->Args({128, 40, 40});

static void BM_View(benchmark::State& state) {
  using namespace kuiper_infer;

  uint32_t channels = 32;
  uint32_t rows = 128;
  uint32_t cols = 128;

  sftensor input = std::make_shared<ftensor>(channels, rows, cols);
  input->Fill(1.f);

  std::vector<sftensor> outputs(1);
  std::vector<sftensor> inputs;
  inputs.push_back(input);

  ViewLayer view_layer({1, 128, 32, 128});
  for (auto _ : state) {
    const auto status = view_layer.Forward(inputs, outputs);
  }
}

BENCHMARK(BM_View);
